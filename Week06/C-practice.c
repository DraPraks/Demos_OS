#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    printf("PID==[%.5d] ==== ======= ==== =======\n", getpid());
    fflush(NULL);

    // write a for loop 3 times
    pid_t val1, val2, val3;
    val3 = val2 = val1 = 1000;  // Initialize all to 1000
    
    // level
    int lvl = 0;
    
    for (int i = 0; i < 3; i++){
        val1 = fork();
        wait(NULL);
        if (val1 == 0) {


        int curr_pid = getpid();
        int curr_ppid = getppid();

        printf("L%d: PID[%.5d] (PPID[%.5d])\n", lvl, (int)curr_pid, (int)curr_ppid);
        lvl++;        
        }
    }

    return 0;
}