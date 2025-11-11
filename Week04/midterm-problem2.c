#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char str_local[100];
    char *str_heap;

    if (scanf("%s", str_local) == 1) {
        str_heap = malloc(strlen(str_local) + 1);
        if (str_heap != NULL) {
            strcpy(str_heap, str_local);
            printf("Original: %s\nCopy: %s\n", str_local, str_heap);
            free(str_heap);
            return 0;
        }
        fprintf("Malloc failed");
        return -1;
    } 
}