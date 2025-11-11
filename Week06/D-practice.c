#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    printf("YEAR1   YEAR2   OTHER\n=====================\n");
    fflush(NULL);

    // write a for loop 3 times
    pid_t pid1, pid2, pid3;
    pid1 = pid2 = pid3 = getpid();  // Initialize all to current PID
    // parent
    printf("[%.5d][%.5d][%.5d]\n", (int)pid1, (int)pid2, (int)pid3);

    fork();
    pid1 = getpid();
    wait(NULL);
    pid2 = getpid();

    if (!fork()) {
        fork();
    }

    pid3 = getpid();
    wait(NULL);
    printf("[%5.5d][%5.5d][%5.5d]\n", (int)pid1, (int)pid2, (int)pid3);

    return 0;
}