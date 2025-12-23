//Write a C program that creates four threads, each running a unique worker function. The threads must run in reverse order starting from thread 4 to thread 1 using semaphores. Each thread prints messages with prefix as shown in the output.

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_WORKERS 4

void* worker(void* arg) {
   int id = *((int*) arg);
   free(arg);

   printf("thread id: %d - doing work\n", id);
   sleep(1);
   printf("thread id: %d - finished\n", id);
   return NULL;
}

int main(void) {
   pthread_t threads[NUM_WORKERS];

   // creating threads to run in parallel

   for (int i = 0; i < NUM_WORKERS; i++) {
      int* id = malloc(sizeof(int)); // get a unique pointer to each thread?
      *id = i + 1; // starts from 1

      // create the thread, note that once pthread_create is called that thread immediately runs its job using worker
      pthread_create(&threads[i], NULL, worker, id);
   }

   for (int i = 0; i < NUM_WORKERS; i++) {
      pthread_join(threads[i], NULL);
   }

   return 0;
}