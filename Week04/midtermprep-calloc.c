#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_STRINGS 5
#define MAX_LEN 51

int main() {
    const char *phrases[NUM_STRINGS] = {"Apple", "Banana", "Cherry", "Date", "Elderberry"};
    char **str_array = calloc(NUM_STRINGS, sizeof(char *));
    int str_len = 0;

    if (str_array == NULL) {
        fprintf(stderr, "calloc failed\n");
        free(str_array);
        return 1;
    }
    size_t total_len = 0;
    // Your code here (allocate per string, strcpy, sum lengths)
    printf("Array contents:\n");

    for (int i = 0; i < NUM_STRINGS; i++) {
        str_array[i] = malloc(strlen(phrases[i]) + 1);
        strcpy(str_array[i], phrases[i]);
        printf("%i: %s\n", i+1, str_array[i]);
        str_len += strlen(str_array[i]);
    }
    printf("Total Length: %i\n", str_len);

    for (int i = 0; i < NUM_STRINGS; i++) {
        free(str_array[i]);
    }

    free(str_array);
    printf("All memory freed.\n");
    return 0;
}