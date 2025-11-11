#include <stdio.h>
#include <sys/stat.h>

int main() {
    const char *filename = "fileku.txt";
    struct stt file_stat; // file info is stored in this struct

    if (stat(filename, &stt) == 0) {
        printf("File Information: %s\n", filename);
        printf("Device ID: %lu\n", (unsigned long)stt.st_dev);
        printf("File Size(bytes): %u\n", (unsigned)stt.st_size);
        printf("Permission (Octal): %o", stt.st_mode & 0777);
        /* Mask with 0777: isolates permission bits */
        return 0;
    } else {
        perror("stat failed");
        return -1;
    }
}