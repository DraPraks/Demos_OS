#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define DIR_NAME "safe_area"
#define DIR_MODE 0700

int main() {
    int mkdir_return = mkdir(DIR_NAME, DIR_MODE);

    if (mkdir_return == 0) {
        printf("Directory '%s' has been succesfully created with %i permissions\n", DIR_NAME, DIR_MODE);
        return 0;
    } 

    if (errno == EEXIST) {
        fprintf(stderr, "Failed to create directory '%s' : path already exists!\n", DIR_NAME);
        return -1;
    }

    if (mkdir_return == -1) {
        fprintf(stderr, "An unknown error occurred while creating directory '%s'\n", DIR_NAME);
        return -1;
    }
}