// advanced log file manager

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
#define MAX_LOG_SIZE 1024 // 1KB for demonstration
#define BACKUP_LOG_FILE "2406453530_logfile.log.bak"
#define BUFFER_SIZE 256

/* feature 1 */

char* get_header() {
    // function should format the header string and return it
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char *header = malloc(256); // allocate memory for the string
    if (header == NULL) {
        return NULL; // handle allocation failure
    }
    sprintf(header, "========= 2406453530_Log %04d-%02d-%02d %02d:%02d:%02d ========\n",
           t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
           t->tm_hour, t->tm_min, t->tm_sec);
    return header;
}

void write_log(const char *logfile, const char *msg) {
    FILE *fd = fopen(logfile, "a");
    if (fd == NULL) {
        // use fprintf to stderr
        fprintf(stderr, "Failed to open log file: %s\n", strerror(errno));
        return;
    }

    char* header = get_header();
    if (header == NULL) {
        fprintf(stderr, "Failed to allocate memory for header\n");
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
    struct tm *t = localtime(&now);
    fprintf(fd, "[%04d-%02d-%02d %02d:%02d:%02d]_2406453530_Log %s\n",
           t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
           t->tm_hour, t->tm_min, t->tm_sec, msg);

    fclose(fd);
}

int main(void) {
    // Example usage of write_log function
    write_log(LOG_FILE, "This is a test log message.");
    return 0;
}

