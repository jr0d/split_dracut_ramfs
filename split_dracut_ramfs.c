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
    char magic[7];
    char buffer[1024]; //General purpose + the size of our haystack
    bool is_cpio_result;
    long pos = 0;


    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    f = fopen(argv[1], "r");

    if (f == NULL) {
        fprintf(stderr, "Could not open file '%s'\n", argv[1]);
        exit(1);
    }

     if (fread(magic, 6, 1, f) < 6) {
         fprintf(stderr, "Error reading signature from file '%s'\n", argv[1]);
         fclose(f);
         exit(1);
    }

    // fseek(f, 0, SEEK_SET);

    printf("%s\n", magic);
    printf("RESULT: %s\n", is_cpio(magic) ? "true" : "false");
    pos = ftell(f);
    printf("POS: %ld\n", pos);

    // is_cpio, scan for TRAILER, and cat out the initramfs
    // if is_cpio is false, exit with error status
    // if TRAILER is not found, exit with error status

    return 0;
}

