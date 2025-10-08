/*
 * Copyright (C) 2020-2023 BinKadal, Sdn. Bhd.
 * http://rahmatm.samik-ibrahim.vlsm.org/
 * This program is free script/software. This program is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# INFO: UTS 2020-1 (midterm)
 * REV03: Thu 26 Oct 2023 19:00
 * REV02: Mon 12 Oct 2020 11:00
 * REV01: Thu 12 Mar 2020 10:00
 * START: Tue 10 Mar 2020 15:00
 */

#include <stdio.h>

int returnInt(int ii) {
   return ii;
}

char returnChar(char cc) {
   return cc;
}

void main(void) {
    int ii=0x41424344;
    
    printf("returnChar=%c\n",
            returnChar((char) ii)); 

      /*
      (char) ii means typecast int to char, take LSB only: 
      0x41424344 -> 0x44 -> 'D'
      then return it as char
      */

    printf("returnChar=%#x\n", 
      (int) returnChar((char) ii)); // typecast char to int
      // returnChar((char) ii) is 0x44 = 'D' but typecast to int so becomes 0x00000044 (0x44)

      /*
      here we first typecast int to char => (char) ii returns 'D' (0x44)
      0x41424344 -> 0x44 -> 'D'
      when we typecast char to int, the char is converted to its ASCII value
      'D' -> 0x44
      */

    printf("returnInt==%c\n",
     (char) returnInt(ii)); // typecast int to char

      /*
      above we return int which is 0x41424344
      then typecast int to char, so only the least significant byte is taken
      0x41424344 -> 0x44 -> 'D'
      */

      // NOTE: ii is never changed here, still 0x41424344

    printf("returnInt==%#x\n",
            returnInt(ii)); // original ii
}

