specs.md

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