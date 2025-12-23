# alfm 

Included files:
- `2406453530_alfm.c`: Annotated source code (with comments for each function).
- `Makefile`: Build script to compile `2406453530_alfm.c` into the `alfm` executable.
- `2406453530_logfile.log`, `2406453530_logfile.log.1`, `2406453530_logfile.log.2`: Sample log files created by the program demonstrating rotation.

How to compile:
1. In WSL or Linux, go to the deliverables folder:

```bash
cd /mnt/c/Users/prako/OneDrive/Documents/Projects/Demos_OS/PA4/deliverables
```

2. Build using `make`:

```bash
make
```

This produces an `alfm` executable in the `deliverables` folder.

How to run (examples):

- Write a new log entry (buffered I/O):
```bash
./alfm --write 2406453530_logfile.log "My log message"
```

- Rotate logs (low-level I/O):
```bash
# explicit size (in bytes)
./alfm --rotate 2406453530_logfile.log 1024

# omitted size -> fallback to default
./alfm --rotate 2406453530_logfile.log
```

- Tail the last N lines (low-level I/O):
```bash
./alfm --tail 2406453530_logfile.log 10
```

- Get file info (stat):
```bash
./alfm --info 2406453530_logfile.log
```

