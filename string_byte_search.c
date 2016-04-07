#include <stdio.h>
#include <stdbool.h>

int main()
{
    size_t index;

    char test1[] = "BEER IS GOOD";
    char haystack[] = "CRAFTBEER IS GOOD! I LOVE IT SO MUCH";

    while(*haystack) {
        if (*haystack == *test1) {
            bool match = true;
            while (*test1){
                match &= *haystack++ == *test1++;
                if (match)
                    continue;
                else
                    break;
            
        }
    }
}
