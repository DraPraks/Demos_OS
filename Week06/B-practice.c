#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    printf("PID==[%.5d] ==== ======= ==== =======\n", getpid());
    fflush(NULL);

    pid_t val1, val2, val3;
    val3 = val2 = val1 = 1000;  // Initialize all to 1000

    val1 = fork();
    wait(NULL);
    val2 = fork();
    wait(NULL);
    
    printf("VAL1=[%.5d] VAL2=[%.5d] VAL3=[%.5d]\n", val1, val2, val3);
    
    return 0;
}