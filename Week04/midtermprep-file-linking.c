#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define TARGET "config.ini"
#define LINKNAME "link_to_config"

// note: int lstat(const char *path, struct stat *buf);

int main() {
    int lstat_ret;
    struct stat info;
    bool is_symlink;
    /* Handle if target file doesn't exist */
    if (access(TARGET, F_OK) == -1) {
        int fd = open("config.ini", O_CREAT | O_RDWR, 0644);
        printf("%s file did not exist, created file with permission 0644\n", TARGET);
        if (fd == -1) {
            perror("open failed\n");
            return -1;
        }
        close(fd);
    }

    if (symlink(TARGET, LINKNAME) == -1) {
        perror("symlink failed");
        return 1;
    }
    // Use lstat and print
    lstat_ret = lstat(LINKNAME, &info);

    if (lstat_ret == -1) {
        perror("lstat failed\n");
        return -1;
    }

    if (S_ISLNK(info.st_mode) != 0) {
        is_symlink = true;
    } else {
        perror("symlink is false, stopping exec\n");
        return -1;
    }

    if (is_symlink == true) {
        printf("Symbolic Link Created: %s -> %s\n", LINKNAME, TARGET);
        printf("Link Size (bytes): %llu\n", (unsigned long long)info.st_size);
        printf("Is Symbolic Link: Yes\n");
        return 0;
    } else {
        perror("Unknown error occurred!\n");
        return -1;
    }
}