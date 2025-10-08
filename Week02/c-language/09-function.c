/*
 * Copyright (C) 2019-2023 BinKadal, Sdn. Bhd.
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# INFO: An introduction to function().
 * REV03: Thu 26 Oct 2023 19:00
 * REV02: Fri 07 Feb 2020 01:00
 * REV01: Tue 10 Dec 2019 10:00
 * START: Tue 26 Feb 2019 12:00
 */

#include <stdio.h>
#include <stdlib.h>

int addone1 (int ii) {
   ii++;
   return ii;
}

int addone2 (int* iptr) {
   // here we dereference the pointer to get the value, then increment the value
   // finally we return the value
   // the original variable will be changed because we are changing the value at the address of the pointer

   // this is called pass by reference
   (*iptr)++;
   return *iptr;
}

int* addone3 (int* iptr) {
   // here we dereference the pointer to get the value, then increment the value
   // finally we return the pointer
   // the original variable will be changed because we are changing the value at the address of the pointer

   // difference from addone2 is that we return the pointer instead of the value
   (*iptr)++;
   return iptr;
}

void main(void) {
   int ii=1;
   printf("Addone1 %d -- %d \n", ii,  addone1(ii));
   printf("Addone1 %d -- %d \n", ii,  addone1(ii));
   printf("Addone1 %d -- %d \n", ii,  addone1(ii));
   printf("Addone2 %d -- %d \n", ii,  addone2(&ii));
   printf("Addone2 %d -- %d \n", ii,  addone2(&ii));
   printf("Addone2 %d -- %d \n", ii,  addone2(&ii));
   printf("Addone3 %d -- %d \n", ii, *addone3(&ii));
   printf("Addone3 %d -- %d \n", ii, *addone3(&ii));
   printf("Addone3 %d -- %d \n", ii, *addone3(&ii));
   system("echo This is echo from system. Cek demo.txt");
   system("echo This is echo from system. Cek demo.txt > demo.txt");
}

