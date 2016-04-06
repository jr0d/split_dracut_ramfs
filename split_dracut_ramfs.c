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
    FILE *f;
    char magic[MAGICSIZE + 1];
    char buffer[1024]; //General purpose + the size of our haystack
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

    printf("%s\n", magic);
    printf("RESULT: %s\n", is_cpio(magic) ? "true" : "false");
    pos = ftell(f);
    printf("POS: %ld\n", pos);

    // is_cpio, scan for TRAILER, and cat out the initramfs
    // if is_cpio is false, exit with error status
    // if TRAILER is not found, exit with error status

    if (!is_cpio(magic)) {
        fprintf(stderr, "File '%s' is missing CPIO signature", argv[1]);
        exit(1);
    }

    size_t s, buffsize_nullzero;
    char *head_ptr;

    buffsize_nullzero = sizeof(buffer) - 2  // Reading binary data, buffer might not contain a nullzero
    while (!feof(f)) {
        buffer[1023] = '\0';
        s = fread(buffer, sizeof(buffer) - 2, 1, f)
    }
    return 0;
}

