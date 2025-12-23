// advanced log file manager - 2406453530_alfm.c 

/*
Goal: manage log files in a linux system using a combination of Low-Level I/O + Buffered I/O.
The program must be able to:

- Write new logs to a specified log file (log name format: NPM_logfile.log, e.g. 241234656_logfile.log).
- Perform automatic log rotation when the file size exceeds a certain limit.
- Display the last contents of the log (similar to tail but simplified).
- Display log file information: size, modification time, owner, and permissions.

Program must have 4 operation modes: 

1. Log Writing Mode (--write)
./alfm --write logfile "log message"
Rules:
If the logfile does not exist → create a new file (log naming format: NPM_logfile.log)
If the logfile already exists → append a new line.

The beginning of each log file must contain:
========= YourNPM_Log YYYY-MM-DD HH:MM:SS ========
Mandatory log format for each line:
[YYYY-MM-DD HH:MM:SS]_NPM_Log log message
Note: use buffered I/O (fopen, fprintf).

2. Log Rotation Mode (--rotate)
./alfm --rotate NPM_logfile.log max_size
Example:
./alfm --rotate NPM_logfile.log 1048576   # rotate if >1MB
Rules:
Use stat() to read file size.
If size > max_size:
Rename NPM_logfile.log → NPM_logfile.log.1 using rename().
If .1 already exists, continue with .2, .3, .4, etc.
Create a new empty NPM_logfile.log.
Note: use low-level I/O (open, write, close).

3. Display Last Lines Mode (--tail)
./alfm --tail filename N
Rules:
Display the last N lines, similar to tail -n.
Must use low-level I/O + lseek().
Optimize so the program does not read the entire file unnecessarily if it is large.


4. File Info Mode (--info)
./alfm --info filename
Use stat() to display:
File size (Bytes)
Last modification time
Owner UID
Permissions (rwx format)


Example output:
File: namafile.txt
Size: 82312 bytes
Last Modified: 2025-01-23 14:22:11
Owner UID: 1000
Permission: rw-r--r--

3. Display Last Lines Mode (--tail)
./alfm --tail filename N
Rules:
Display the last N lines, similar to tail -n.
Must use low-level I/O + lseek().
Optimize so the program does not read the entire file unnecessarily if it is large.


4. File Info Mode (--info)
./alfm --info filename
Use stat() to display:
File size (Bytes)
Last modification time
Owner UID
Permissions (rwx format)


Example output:
File: namafile.txt
Size: 82312 bytes
Last Modified: 2025-01-23 14:22:11
Owner UID: 1000
Permission: rw-r--r--

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#define LOG_FILE "2406453530_logfile.log"
#define MAX_LOG_SIZE 1024 // 1kb
#define BACKUP_LOG_FILE "2406453530_logfile.log.bak"
#define BUFFER_SIZE 256

/* feature 1 */

char* format_time(time_t t) {
    struct tm *tm = localtime(&t);
    if (tm == NULL) return NULL;

    size_t bufsize = 64; // initial buffer size; definitely large enough for our formatting
    char *str = malloc(bufsize);
    if (str == NULL) {
        return NULL;
    }

    int needed = snprintf(str, bufsize, "%04d-%02d-%02d %02d:%02d:%02d",
           tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
           tm->tm_hour, tm->tm_min, tm->tm_sec);
    if (needed < 0) {
        free(str);
        return NULL;
    }
    if ((size_t)needed >= bufsize) {
        size_t newsize = (size_t)needed + 1;
        char *newstr = realloc(str, newsize);
        if (newstr == NULL) {
            free(str);
            return NULL;
        }
        str = newstr;
        snprintf(str, newsize, "%04d-%02d-%02d %02d:%02d:%02d",
           tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
           tm->tm_hour, tm->tm_min, tm->tm_sec);
    }
    return str;
}

char* get_header() {
    // function should format the header string and return it
    time_t now = time(NULL);
    char *time_str = format_time(now);
    if (time_str == NULL) {
        return NULL;
    }
    char *header = malloc(256); // allocate memory for the string
    if (header == NULL) {
        free(time_str);
        return NULL; // handle allocation failure
    }
    sprintf(header, "========= 2406453530_Log %s ========\n", time_str);
    free(time_str);
    return header;
}

void write_log(const char *logfile, const char *msg) {
    FILE *fd = fopen(logfile, "a");
    if (fd == NULL) {
        // use perror to print error for fd
        perror("Failed to open log file");
        return;
    }

    char* header = get_header();
    if (header == NULL) {
        perror("Failed to allocate memory for header");
        fclose(fd);
        return;
    }
    
    // check if file is empty
    fseek(fd, 0, SEEK_END);
    if (ftell(fd) == 0) {
        // if empty, write the header
        fprintf(fd, "%s", header);
    }
    free(header);

    // write the log message
    time_t now = time(NULL);
    char *time_str = format_time(now);
    if (time_str == NULL) {
        perror("Failed to format time");
        fclose(fd);
        return;
    }
    fprintf(fd, "[%s]_2406453530_Log %s\n", time_str, msg);
    free(time_str);

    fclose(fd);
}

// feature 2 - log rotation mode - use low lvl I/O

/*
<summary>
If the log file size exceeds max_size, rotate the log file.
Old log file is renamed to logfile.1, logfile.2, etc.
make a new empty log file after rotation.
</summary>
*/
void rotate_log(const char *logfile, off_t max_size) {
    struct stat st;

    if (stat(logfile, &st) == -1) {
        perror("Failed to get file status");
        return;
    }

    if (st.st_size <= max_size) {
        // no need to rotate
        fprintf(stdout, "Log file size (%ld bytes) is within limit (%ld bytes), no rotation needed.\n", st.st_size, max_size);
        return;
    }

    // find the next available backup filename
    char backup_filename[256];
    int index = 1;
    while (1) {
        snprintf(backup_filename, sizeof(backup_filename), "%s.%d", logfile, index);
        // if file doesn't exist, we can use this name, break the loop
        if (stat(backup_filename, &st) == -1) {
            break;
        }
        index++;
    }

    // rename current log file to backup filename, must use low lvl I/O
    if (rename(logfile, backup_filename) == -1) {
        perror("Failed to rotate log file");
        return;
    }

    // create a new empty log file
    int fd = open(logfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to create new log file");
        return;
    }
    close(fd);

    fprintf(stdout, "Log file rotated to %s\n", backup_filename);
    return;
}

// feature 3 - display last lines mode

void display_last_lines(const char *filename, int n) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    // get file size (for offset calculation)
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("Failed to seek to end");
        close(fd);
        return;
    }

    // read backwards to find the start of the last n lines
    int count = 0;
    off_t pos = file_size - 1;
    char ch;

    // skip trailing newline at EOF, avoid counting an extra empty line
    if (pos >= 0) {
        if (lseek(fd, pos, SEEK_SET) == -1) {
            perror("Failed to seek");
            close(fd);
            return;
        }
        if (read(fd, &ch, 1) == 1 && ch == '\n') {
            pos--;
        }
    }

    while (pos >= 0) {
        // SEEK_SET sets offset to a position relative to beginning of file regardless of fd position
        if (lseek(fd, pos, SEEK_SET) == -1) {
                perror("Failed to seek");
            close(fd);
            return;
        }

        // error check for read
        if (read(fd, &ch, 1) != 1) {
            break;  // Read error or EOF
        }

        // count lines by checking newline
        if (ch == '\n') {
            count++;
            if (count >= n) {
                pos++;  // move to start of next line (after newline)
                break;
            }
        }
        pos--;
    }

        // assert pos valid (>=0) before seeking; if pos < 0, print whole file
        if (pos < 0) {
            pos = 0;
        }

    // read and print from pos (start of last n lines) to end
    if (lseek(fd, pos, SEEK_SET) == -1) {
            perror("Failed to seek to print position");
        close(fd);
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    close(fd);
}

// feature 4 - file info mode

void display_file_info(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == -1) {
           perror("Failed to get file status");
        return;
    }

    // file size
    printf("File: %s\n", filename);
    printf("Size: %ld bytes\n", st.st_size);

    // last mod time
    char *time_str = format_time(st.st_mtime);
    if (time_str == NULL) {
           perror("Failed to format time");
        return;
    }
    printf("Last Modified: %s\n", time_str);
    free(time_str);

    // Owner UID
    printf("Owner UID: %d\n", st.st_uid);

    // Permissions in rwx format
    char perms[10];
    perms[0] = (st.st_mode & S_IRUSR) ? 'r' : '-';
    perms[1] = (st.st_mode & S_IWUSR) ? 'w' : '-';
    perms[2] = (st.st_mode & S_IXUSR) ? 'x' : '-';
    perms[3] = (st.st_mode & S_IRGRP) ? 'r' : '-';
    perms[4] = (st.st_mode & S_IWGRP) ? 'w' : '-';
    perms[5] = (st.st_mode & S_IXGRP) ? 'x' : '-';
    perms[6] = (st.st_mode & S_IROTH) ? 'r' : '-';
    perms[7] = (st.st_mode & S_IWOTH) ? 'w' : '-';
    perms[8] = (st.st_mode & S_IXOTH) ? 'x' : '-';
    perms[9] = '\0';
    printf("Permission: %s\n", perms);
}

int main(int argc, char *argv[]) {
    // handle --write
    if (argc == 4 && strcmp(argv[1], "--write") == 0) {
        const char *logfile = argv[2];
        const char *msg = argv[3];
        write_log(logfile, msg);
        return 0;
    }

    // handle --rotate
    if (strcmp(argv[1], "--rotate") == 0) {
        // check argument count: must have at least logfile, max_size is optional
        if (argc < 3 || argc > 4) {
            fprintf(stderr, "Usage: %s --rotate <logfile> [max_size]\n", argv[0]);
            return 1;
        }
        const char *logfile = argv[2];
        off_t max_size;
        if (argc == 4) {
            // user provided max_size, parse it
            max_size = atoll(argv[3]);
        } else {
            // no max_size provided, fall back to default 1kb
            max_size = MAX_LOG_SIZE;
        }
        rotate_log(logfile, max_size);
        return 0;
    }

    // handle --tail
    if (argc == 4 && strcmp(argv[1], "--tail") == 0) {
        const char *filename = argv[2];
        int n = atoi(argv[3]);
        display_last_lines(filename, n);
        return 0;
    }

    // handle --info
    if (argc == 3 && strcmp(argv[1], "--info") == 0) {
        const char *filename = argv[2];
        display_file_info(filename);
        return 0;
    }
    
    return 0;
}

