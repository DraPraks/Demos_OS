/*
 * Copyright (C) 2016-2023 BinKadal, Sdn. Bhd.
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# INFO: An introduction to function printf().
 * REV07: Sun 17 Sep 2023 08:00
 * REV06: Fri  7 Feb 2020 01:00
 * REV05: Tue 17 Sep 2019 18:00
 * REV02: Tue 28 Aug 2018 13:00
 * START: Thu 15 Sep 2016 11:00
 */

#include <stdio.h>

void main (void) {
   int   ii   = 74;             // Integer

   printf("(%%d)    ii = (%d)\n",    ii); // %d = decimal integer, %% means print a single %
   printf("(%%5d)   ii = (%5d)\n",   ii); // checks if ii is > 5, if less: pad with spaces on the left until 5 length
   printf("(%%1d)   ii = (%1d)\n",   ii); // checks if ii is > 1, if less: pad with spaces on the left until 1 length
   printf("(%%-5d)  ii = (%-5d)\n",  ii); // checks if ii is > 5, if less: pad with spaces on the right until 5 length
   printf("(%%5.5d) ii = (%5.5d)\n", ii); // checks if ii is > 5, if less: pad with zeros on the left until 5 length
   printf("(%%5.3d) ii = (%5.3d)\n", ii); // checks if ii is > 3, if less: pad with zeros on the left until 3 length
   printf("(%%c)    ii = %c\n",      ii); // %c = character
   printf("(%%x)    ii = %x\n",      ii); // %x = hexadecimal integer
   printf("(%%#x)   ii = %#x\n",     ii); // %#x = hexadecimal integer with 0x prefix
   printf("(%%X)    ii = %X\n",      ii); // %X = hexadecimal integer (uppercase)
   printf("(%%#X)   ii = %#X\n",     ii); // %#X = hexadecimal integer (uppercase) with 0X prefix
   printf("(%%o)    ii = %o\n",      ii); // %o = octal integer
   printf("(%%#o)   ii = %#o\n",     ii); // %#o = octal integer with 0 prefix
}

