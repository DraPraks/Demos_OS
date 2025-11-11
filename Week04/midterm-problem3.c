#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int num;
    int *arr;
    int sum = 0;

    // get input from user 
    printf("input the number of integers:\n");
    if (scanf("%i", &num) == 1 && num > 0) {
        /* allocate memory chunk */
        arr = mmap(
            NULL, num *sizeof(int),
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1, 0
        );

        if (arr == MAP_FAILED) {
            fprintf(stderr, "mmap err\n");
            return -1;
        } 

        printf("Enter %i integers:\n", num);
        for (int i = 0; i < num; i++) {
            if (scanf("%i", &arr[i]) != 1) {
                munmap(arr, num*sizeof(int));
                fprintf(stderr, "mmap err\n");
                return -1;
            }
        }

        for (int i = 0; i < num; i++) {
            sum += arr[i];
        }

        printf("Sum = %i", sum);
        munmap(arr, num*sizeof(int));
        return 0;
    }
}