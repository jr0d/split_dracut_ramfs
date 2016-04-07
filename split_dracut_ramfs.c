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
#define MAGICSIZE sizeof(MAGIC) - 1
#define TRAILER "TRAILER!!!" // Hmm...
#define TRAILERSIZE sizeof(TRAILER)
#define SEARCHBUFFERSIZE TRAILERSIZE * 512

bool is_cpio(const char * test) {
    size_t s = sizeof(MAGIC) + 1;
    int result;

    result = strncmp(test, MAGIC, s);
    
    if (result != 0)
        return false;
    
    return true;
}

int search(char * haystack) {
    int index, result;

    for (index = 0; index < SEARCHBUFFERSIZE; index += TRAILERSIZE) {
        result = strncmp(&haystack[index], TRAILER, TRAILERSIZE);
        if (result==0)
            return index;
    }

    return -1;

}

int main(int argc, char **argv)
{
    FILE *f;
    char magic[MAGICSIZE + 1];
    char buffer[SEARCHBUFFERSIZE]; //General purpose + the size of our haystack
    long pos;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    f = fopen(argv[1], "r");

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
        fprintf(stderr, "File '%s' is missing CPIO signature", argv[1]);
        exit(1);
    }

    int search_result;

    while (!feof(f)) {
        pos = ftell(f);
        fread(buffer, 1, SEARCHBUFFERSIZE, f);
        search_result = search(buffer);
        if (search_result >= 0) {
            
        }
    }
    return 0;
}

