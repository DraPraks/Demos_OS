/*
 * Copyright (C) 2016-2023 BinKadal, Sdn. Bhd.
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * REV09: Fri 27 Oct 2023 07:00
 * REV08: Mon 30 Mar 2020 07:00
 * REV06: Wed 29 Aug 2018 16:00
 * REV01: Wed 03 May 2017 20:00
 * START: Mon 24 Oct 2016 09:00
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void main(void) {
   char* iAM="PARENT";
  
   printf("PID[%d] PPID[%d] (START:%s)\n", getpid(), getppid(), iAM);
   // ↑ This prints ONCE (only the parent executes this before fork)
   
   if (fork() > 0) {    // ← FORK HAPPENS HERE!
                        //   Parent gets child's PID (e.g., 1235 > 0) → takes this branch
                        //   Child gets 0 (0 is NOT > 0) → skips this branch
      // === PARENT ONLY ===
   sleep(1);
      printf("PID[%d] PPID[%d] (IFF0:%s)\n", getpid(), getppid(), iAM);
      
   } else {
      // === CHILD ONLY ===
      iAM="CHILD";
      printf("PID[%d] PPID[%d] (ELSE:%s)\n", getpid(), getppid(), iAM);
   }
   
   // Both processes execute this:
   printf("PID[%d] PPID[%d] (STOP:%s)\n", getpid(), getppid(), iAM);
}

/*
# INFO: System call "fork()"
# INFO:                   Compare "01-fork.c", "02-fork.c", and "03-fork.c"
# INFO:                   See where the line "LOOK THIS" is.
 */

