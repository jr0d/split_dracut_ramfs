#include <stdio.h>

int main() {
    char separator[20] = "Big Fish!";
    char * separator_head = separator;
    char * separator_current = separator_head;


    printf("%s\n", separator);
    printf("%s\n", separator_head);
    separator_current++;
    printf("%s\n", separator_current);
    separator_current = separator_head;
    printf("%s\n", separator_current);

}
