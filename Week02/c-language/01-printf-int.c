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
   printf("(%%5d)   ii = (%5d)\n",   ii); // Width 5: minimum 5 chars, pad left with spaces if shorter
   printf("(%%1d)   ii = (%1d)\n",   ii); // Width 1: minimum 1 char, pad left with spaces if shorter
   printf("(%%-5d)  ii = (%-5d)\n",  ii); // Width 5, left-align: pad right with spaces to 5 chars if shorter
   printf("(%%5.5d) ii = (%5.5d)\n", ii); // Width 5, precision 5: minimum 5 digits (zero-pad left if fewer), then pad field to 5 chars
   printf("(%%5.3d) ii = (%5.3d)\n", ii); // Width 5, precision 3: minimum 3 digits (zero-pad left if fewer), then pad field to 5 chars with spaces
   printf("(%%c)    ii = %c\n",      ii); // %c = character
   printf("(%%x)    ii = %x\n",      ii); // %x = hexadecimal integer
   printf("(%%#x)   ii = %#x\n",     ii); // %#x = hexadecimal integer with 0x prefix
   printf("(%%X)    ii = %X\n",      ii); // %X = hexadecimal integer (uppercase)
   printf("(%%#X)   ii = %#X\n",     ii); // %#X = hexadecimal integer (uppercase) with 0X prefix
   printf("(%%o)    ii = %o\n",      ii); // %o = octal integer
   printf("(%%#o)   ii = %#o\n",     ii); // %#o = octal integer with 0 prefix

   unsigned int uii = 1009312049; // 0x3C3D3E3F
   printf("(%%u)    uii = %u\n",      uii); // %u = unsigned integer
   printf("(%%#X)   uii = %#X\n",     uii); // %#X = hexadecimal integer (uppercase) with 0X prefix
   printf("(%%#16.16lX) uii = %#16.16lX\n", (unsigned long) uii); // Width 16, precision 16: minimum 16 digits (zero-pad left if fewer), long int, uppercase hex with 0X prefix
}

/*
Overall, the general format is %[flags][width][.precision][length][type]
In the case of %#16.16lX:
- %: Indicates the start of a format specifier.
- #: A flag that adds a prefix to the output (0X for hexadecimal).
- 16: The minimum field width. The output will be at least 16 characters wide
- .16: The precision specifier, indicating at least 16 digits (zero-padded if necessary).
- l: A length modifier indicating that the argument is a long int.
- X: The type specifier indicating that the argument should be printed as an uppercase hexadecimal integer

Execution:
1. Pad the number with leading zeros to ensure it has at least 16 digits.
2. Add the 0X prefix (due to # flag).
3. Pad the entire output (including prefix) to at least 16 characters with spaces on the left (default right-align).

Generalized Execution Flow for printf Format Specifiers:
1. **Argument Conversion**: Interpret the argument based on length modifier (e.g., l for long) and type (e.g., X for hex). Convert to the base string representation (e.g., decimal, hex digits).
2. **Precision Application**: 
   - For integers (d, x, X, o, u): Pad with leading zeros to meet minimum digit count. Does not truncate.
   - For floats (f, e, g): Controls decimal places or significant digits.
   - For strings (s): Limits maximum characters (truncates if longer).
   - If no precision, no change.
3. **Flag Application**: Apply flags in order (e.g., # adds prefixes like 0x/0X, + adds signs, 0 changes padding to zeros).
4. **Width Application**: Ensure the output is at least the specified width. Pad with spaces (or zeros if 0 flag) on left (right-align) or right (if - flag for left-align). Width includes prefixes/signs added by flags.
5. **Output**: Print the final formatted string.

This flow ensures precision modifies the core value first, then flags add decorations, and width handles alignment/layout.
*/