/* Implementing skipcpio.c from the dracut project from scratch (mostly)
 * there is nothing wrong with the original implimentation, so I
 * am using this as an opportunity to re-learn come C
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define MAGIC "070701"
#define MAGICSIZE strlen(MAGIC)
#define TRAILER "TRAILER!!!" // Hmm...
#define TRAILERSIZE strlen(TRAILER)
#define BCOUNT (size_t) 512  // How many bytes to stuff

bool is_cpio(const char * test) {
    size_t s = sizeof(MAGIC) + 1;
    int result;

    result = strncmp(test, MAGIC, s);
    
    if (result != 0)
        return false;
    
    return true;
}

int search(char * haystack, size_t haystack_size) {
    int index, result;

    for (index = 0; index < haystack_size; index += TRAILERSIZE) {
        result = strncmp(&haystack[index], TRAILER, TRAILERSIZE);
        printf("INDEX: %i\n", index);
        printf("TEST: %s, SIZE: %u\n", &haystack[index], strnlen(&haystack[index], 15));
        if (result==0)
            return index;
    
        if (result > -1 && result < 4)
            printf("POSSIBLE: %s\n", &haystack[index]);
    }


    return -1;
}


int main(int argc, char **argv)
{
    printf("%i , %i\n", TRAILERSIZE, BCOUNT);
    printf("%i\n", TRAILERSIZE * BCOUNT);
    printf("%li\n", MAGICSIZE * (size_t) 512);
    FILE *f;
    char magic[MAGICSIZE + 1];
    char buffer[1024]; //General purpose + the size of our haystack
    size_t pos, endpos;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
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

    pos = MAGICSIZE;

    if (!is_cpio(magic)) {
        fprintf(stderr, "File '%s' is missing CPIO signature\n", argv[1]);
        fclose(f);
        exit(1);
    }

    int index;
    long trailer_index;

    char * separator_head = TRAILER;
    char * separator_current = separator_head;

    while (!feof(f)) {
        unsigned int s;
        int result;
        size_t buffer_index, match_index;
        char minibuf[TRAILERSIZE + 1];
        buffer_index = 0;

        pos = ftell(f);
        printf("POS: %li\n", pos);
        s = fread(buffer, 1, 1024, f);
        endpos = ftell(f);
        
        if (s) {
            bool match = true;
            while (buffer_index <= s) {
                if (*buffer == *separator_current) {
                    match_index = 0;
                    while (*seperator_current) {
                        if (buffer_index == s) {
                            fread(minibuf, 1, TRAILERSIZE-match_index, f);
                            buffer = minibuf; // Buffers need to pointers... not arrays
                        match &= *buffer++ == *seperator_current++;
                        buffer_index++;
                        if (!match) {
                            separator_current = separator_head;
                            break;
                        }
                buffer_index++;
            }
    }

    fclose(f);
    exit(0);
    if (!trailer_index) {
        fprintf(stderr, "Could not find '%s' in file, perhaps you don't need this?\n", TRAILER);
        exit(1);
    }

    fprintf(stdout, "JOY: %li\n", trailer_index);
    return 0;
}

