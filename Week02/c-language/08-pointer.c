/*
 * Copyright (C) 2019-2023 BinKadal, Sdn. Bhd.
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# INFO: An introduction to pointers.
 * REV02: Thu 26 Oct 2023 19:00
 * REV01: Fri 07 Feb 2020 01:00
 * START: Tue 26 Feb 2019 12:00
 */

#include <stdio.h>
#include <signal.h>

void main (void) {
   char    cca='a';
   printf ("Print %%c  cca: %c\n", cca);
   char    ccb='b';
   printf ("Print %%c  ccb: %c\n", ccb);
   char*   ptr=&cca;
   printf ("Print %%c  *ptr: %c\n", *ptr);
   ptr=&ccb;
   printf ("Print %%c  *ptr: %c\n", *ptr);
   char    array[]="ABCDEF";
   printf ("Print %%s  array: %s\n", array);
   ptr=array;
   printf ("Print %%c  *ptr: %c\n", *ptr);
   ptr=&array[0];
   printf ("Print %%c  *ptr: %c\n", *ptr);
   ptr=&array[1];
   printf ("Print %%c  *ptr: %c\n", *ptr);
   ptr=ptr+1;
   printf ("Print %%c  *ptr: %c\n", *ptr);
   ptr++;
   printf ("Print %%c  *ptr: %c\n", *ptr);

   // infinite loop demonstration:
   // while (1) {
   //    printf ("Infinite loop: *ptr = %c\n", *ptr);
   //    ptr++;
   // }

   // that will create segmentation fault after ptr goes beyond array boundary, core dumped means program crashed
   // to force the program to continue printing outside of array bounds, we can code:

   // UNCOMMENT IF YOU WANT TO IGNORE SEGMENTATION FAULT
   
   // signal(SIGSEGV, SIG_IGN); // ignore segmentation fault signal

   // while (1) {
   //    printf ("Infinite loop: *ptr = %c\n", *ptr);
   //    ptr++;
   // }
}

