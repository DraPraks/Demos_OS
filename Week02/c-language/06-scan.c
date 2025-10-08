/*
 * Copyright (C) 2013-2023 BinKadal, Sdn. Bhd.
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# INFO: An introduction to function scanf().
 * REV05: Thu 26 Oct 2023 19:00
 * REV04: Sun 23 Feb 2020 16:00
 * REV03: Fri 07 Feb 2020 01:00
 * REV02: Tue 28 Aug 2018 13:00
 * START: Tue 01 Jan 2013 00:00
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

int main(void) { 
   char buf[10];
   printf("Write a string: ");
   scanf("%s",buf);
   printf("Isi buffer = %s\n",buf);

   // this does not protect against buffer overflow
   // if user inputs more than 9 characters, it will overflow
   // to protect against buffer overflow, use this instead:
   printf("Write a string (max 9 chars): ");
   scanf("%9s",buf); // limit input to 9 characters, %s means string, %9s means max 9 characters
   printf("Isi buffer = %s\n",buf);

   int ch;
   while ((ch = getchar()) != '\n' && ch != EOF) {
      // discard the rest of the line, including the trailing newline
   }

   // now lets use malloc and realloc to allocate memory dynamically

   char *buffer = NULL;

   size_t init_size = 10; // initial size
   size_t curr_size = init_size; // current size
   size_t max_size = 1000; // maximum size we want to allocate
   size_t curr_len = 0; // current length of string in buffer

   buffer = (char *)malloc(init_size); // allocate initial buffer
   if (buffer == NULL) {
      fprintf(stderr, "Failed to allocate memory\n");
      return -1;
   }

   printf("Write a string (max %zu chars): ", max_size); // %zu is for size_t
   while (1) {
      if (fgets(buffer + curr_len, curr_size - curr_len, stdin) == NULL) {
         break;
      }
      size_t read_len = strlen(buffer + curr_len);
      curr_len += read_len;
      if (curr_len > 0 && buffer[curr_len - 1] == '\n') {
         break;
      }
      if (curr_len >= curr_size - 1) {
         if (curr_size >= max_size) {
            buffer[curr_len] = '\0';
            break;
         }
         size_t new_size = curr_size * 2; // double the size
         if (new_size > max_size) {
            new_size = max_size; // cap to max size
         }
         char *temp = (char *)realloc(buffer, new_size);
         if (temp == NULL) {
            fprintf(stderr, "Failed to reallocate memory\n");
            free(buffer);
            return -1;
         }
         buffer = temp;
         curr_size = new_size;
      }
   }

   printf("Final buffer: %s\n", buffer);
   printf("Final buffer length: %zu\n", strlen(buffer));
   printf("Final buffer memory size: %zu\n", curr_size);
   printf("Final buffer memory location: %p\n", (void*)buffer);
   printf("Final buffer memory location + length: %p\n", (void*)(buffer + strlen(buffer)));
   free(buffer);
   return 0;
}

/*
 * TAKE NOTE(AII)
 * scanf() reads formatted data from standard input 
 * and then writes the results into the arguments given
 * 
 * the line scanf("%s",buf); will read string inputted by
 * user and write the value to variable buf
 */

