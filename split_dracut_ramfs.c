/* Implementing skipcpio.c from the dracut project from scratch (mostly).
 * there is nothing wrong with the original implementation. I am treating
 * this as an opportunity to re-learn some C
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAGIC "070701"
#define MAGICSIZE strlen(MAGIC)
#define TRAILER "TRAILER!!!" // Hmm...
#define TRAILERSIZE strlen(TRAILER)


bool is_cpio(const char * test) {
    size_t s = sizeof(MAGIC) + 1;
    int result;

    result = strncmp(test, MAGIC, s);
    
    if (result != 0)
        return false;
    
    return true;
}


int main(int argc, char **argv)
{
    FILE *f, *output;
    char magic[MAGICSIZE + 1];
    char *buffer;
    char *separator_current;

    buffer = (char *) malloc(1024);

    size_t s;
    size_t pos;
    size_t target_index = 0;
    
    char * separator_head = TRAILER;
    char * buffer_head =  buffer;
    
    separator_current = separator_head;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <packed initrd> <output>\n", argv[0]);
        exit(1);
    }

    f = fopen(argv[1], "rb");

    if (f == NULL) {
        fprintf(stderr, "Could not open file '%s'\n", argv[1]);
        exit(1);
    }

    if (fread(magic, 1, MAGICSIZE, f) < MAGICSIZE) {
        fprintf(stderr, "Error reading signature from file '%s'\n", argv[1]);
        fclose(f);
        exit(1);
    }

    if (!is_cpio(magic)) {
        fprintf(stderr, "File '%s' is missing CPIO signature\n", argv[1]);
        fclose(f);
        exit(1);
    }


    
    while (!feof(f)) {
        bool match = false;

        size_t buffer_index;
        size_t match_index = 0;
                
        buffer_index = 0;

        pos = (size_t) ftell(f);
        s = fread(buffer, 1, 1024, f);

        if (!s) {
            fprintf(stderr, "Error reading from file? Could this ever happen due to EOF? Maybe, if things were perfectly aligned");
            fclose(f);
            exit(1);
        }

        while (buffer_index <= s) {
            if (*buffer == *separator_current) {
                size_t matches = 1;
                match = true;
                match_index = buffer_index;
                buffer++;
                separator_current++;
                while (*separator_current) {
                    if (buffer_index == s)
                        /* Untested:
                         * If we hit the end of our buffer during a match cycle, grab some more bits from the file
                         * NOTE: I need to reset the buffer to buffer_head to avoid over running
                         * I should probably overwrite 's' as well.
                         */
                        fread(buffer, 1, TRAILERSIZE-matches, f);
                    match &= *buffer == *separator_current;
                    buffer_index++;
                    if (!match) {
                        separator_current = separator_head;
                        break; //goto?
                    }
                    buffer++;
                    matches++;
                    separator_current++;
                }
            }

            if (match) {
                target_index = pos + match_index;
                break;
            }
            buffer++;
            buffer_index++;
        }
        buffer = buffer_head;

        if (match) {
            break;
        }

    }

    if (!target_index) {
        fprintf(stderr, "Could not find '%s' in file, perhaps you don't need this?\n", TRAILER);
        exit(1);
    }

    fseek(f, target_index + TRAILERSIZE, SEEK_SET);
    pos = target_index + TRAILERSIZE;
    while (!feof(f)) {
        s = fread(buffer, 1, 1024, f);
        size_t i;
        for (i = 0; i < s && buffer[i] == 0; i++);
        if (buffer[i] != 0) {
            pos += i;
            fseek(f, pos, SEEK_SET);
            break;
        }
    }

    fprintf(stdout, "TRAILER HEAD: %li, FILE POS: %zu\n", target_index, pos);

    if ((output = fopen(argv[2], "w")) == NULL) {
        fprintf(stderr, "Could not open file '%s'\n", argv[2]);
        exit(1);
    }

    fseek(f, pos, SEEK_SET);

    while(!feof(f)) {
        s = fread(buffer, 1, 1024, f);
        fwrite(buffer, 1, s, output);
    }
    fclose(f);
    fclose(output);
    return 0;
}
