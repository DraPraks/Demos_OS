/*
 * Copyright (C) 2016-2023 BinKadal, Sdn. Bhd.
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# INFO: TOP (Table of Processes)
 * REV19: Fri 27 Oct 2023 07:00
 * REV10: Tue 30 Mar 2021 18:00
 * REV07: Fri 26 Mar 2021 22:00
 * REV06: Thu 25 Mar 2021 13:00
 * REV05: Wed 27 Feb 2019 19:00
 * START: Mon 03 Oct 2016 09:00
 */

// This program demonstrates dynamic memory allocation in C and how it affects
// the memory usage of a process as reported by the 'top' command.
// It allocates different sizes of memory and monitors the process's memory footprint.

#define TOKEN  "OS232W05"
// #define MSTARTS 0x16BE5
#define MSTARTS 0x16BE4
// #define MSTARTS 0xDE5
// #define MSTARTS 0xDE4
// #define MSTARTS 0x3E4
// #define MSTARTS 0x1E5
// #define MSTARTS 0x1E4

// These MSIZE constants define various memory allocation sizes in hexadecimal.
// They represent different amounts of memory to allocate in bytes.
// For example, MSIZE00 = 0x1E608 = 124936 bytes.
#define MSIZE14 0x40609
#define MSIZE13 0x40609
#define MSIZE12 0x40608
#define MSIZE11 0x40608
#define MSIZE10 0x20FE8
#define MSIZE09 0x20FE8
#define MSIZE08 0x1F609
#define MSIZE07 0x1F609
#define MSIZE06 0x1F608
#define MSIZE05 0x1F608
#define MSIZE04 0x1E609
#define MSIZE03 0x1E609
#define MSIZE02 0x1E609
#define MSIZE01 0x1E608
#define MSIZE00 0x1E608
#define LINE   75  // Number of 'x' characters to print in a line for visual separation
#define MAXSTR 80  // Maximum string length for buffers

// Include necessary header files for standard I/O, memory management, system calls, and string operations
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

// Define a type alias for unsigned char pointer, commonly used for byte-level operations
typedef  unsigned char* uChrPtr;

// Function prototype for chktoken, which likely validates the TOKEN constant
void     chktoken (uChrPtr token);

// Function to print a line of 'x' characters for visual separation in output
void printLine(int line) {
   while(line-- > 0) putchar('x');  // Print 'x' characters in a loop
   putchar('\n');  // End with a newline
   fflush(NULL);   // Flush the output buffer to ensure immediate display
}

// Global array of unsigned char pointers. MSTARTS defines its size.
// This might be used for global memory allocation demonstrations, but isn't used in main.
uChrPtr GlobalChar[MSTARTS];

// Main function: entry point of the program
void main(void) {
   // Array of memory sizes to allocate in the loop
   int   msize[] = {MSIZE00, MSIZE01, MSIZE02, MSIZE03, MSIZE04, 
                    MSIZE05, MSIZE06, MSIZE07, MSIZE08, MSIZE09,
                    MSIZE10, MSIZE11, MSIZE12, MSIZE13, MSIZE14};
   int   ii, jj;  // Loop counters
   int   myPID    = (int) getpid();  // Get the process ID of this program
   char  strSYS1[MAXSTR], strOUT[MAXSTR];  // Buffers for system commands and output
   char* chrPTR;  // Pointer for memory manipulation
   char* chrStr;  // Pointer to allocated memory

   // Print a separator line
   printLine(LINE);
   // Validate the token (likely for course verification)
   chktoken(TOKEN);
   // Print another separator line
   printLine(LINE);

   // Construct a command to run 'top' for this process, showing the last 5 lines
   sprintf(strSYS1, "top -b -n 1 -p%d | tail -5", myPID);
   // Execute the command to show initial process state
   system (strSYS1);
   // Construct a command to get just the last line of top output (memory info)
   sprintf(strSYS1, "top -b -n 1 -p%d | tail -1", myPID);
   
   /*
   sprintf is used to format and store a command string; it allows C code to create cmd strings
   we then use system() or popen() to execute these commands and capture their output.
   the output is then read into strOUT for further processing.
   strSYS1 = contains the command to be executed
   strOUT = contains the output of the command
   */
   printf("PART 1\n");
   printLine(LINE);
   
   // Loop through each memory size
   for (ii=0; ii < (sizeof(msize)/sizeof(int)); ii++){
      // Allocate memory of the specified size
      chrStr = malloc(msize[ii]);
      // Run top command to get current memory usage
      FILE* filePtr=popen(strSYS1, "r");
      fgets(strOUT, sizeof(strOUT)-1, filePtr);
      // fgets reads a line from the command output into strOUT
      pclose(filePtr);
      // Remove the trailing newline from the output
      strOUT[(int) strlen(strOUT)-1]='\0';
      // Print the top output along with the allocated memory size in hex
      printf("%s [%X]\n", strOUT, msize[ii]);
      // Free the allocated memory
      free(chrStr);
   }
/*
   printf("\nPART 2\n");
   printLine(LINE);
   for (ii=0; ii < (sizeof(msize)/sizeof(int)); ii++){
      // Allocate memory and get a pointer to it
      chrPTR = chrStr = malloc(msize[ii]);
      // Fill the allocated memory with 'x' characters
      // This actually uses the memory, unlike PART 1 which just allocates it
      for (jj=0;jj<msize[ii];jj++)
         *chrPTR++='x';  // Increment pointer and assign 'x'
      // Run top to check memory usage after filling the memory
      FILE* filePtr=popen(strSYS1, "r");
      fgets(strOUT, sizeof(strOUT)-1, filePtr);
      pclose(filePtr);
      strOUT[(int) strlen(strOUT)-1]='\0';
      printf("%s [%X]\n", strOUT, msize[ii]);
      free(chrStr);  // Free the memory
   }
*/
}

