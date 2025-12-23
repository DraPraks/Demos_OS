/*
 * Copyright (C) 2015-2023 BinKadal, Sdn. Bhd.
 * This program is free script/software. This program is distributed in the 
 * hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * REV08: Sat 28 Oct 2023 10:00
 * REV07: Mon 13 Apr 2020 07:57:03 WIB 2020
 * REV05: Wed 29 Aug 2018 18:34:21 WIB 2018
 * REV01: Wed 02 Nov 2016 11:49:39 WIB 2016
 * START: Wed 30 Sep 2015 XX:XX:XX UTC 2015
 */

#include <stdio.h>
#include <stdlib.h>

/*
steps to make semaphores for multithreading in C

1. Define your workers.
workers should be in the form of void* workername(void* arg)

2. Define your semaphore/list of semaphores
example semaphore: 
```
sem_t semaphore <- this will be semaphore object
```

example list of semaphores:
sem_t list_semaphores[int n] <- this will define a list of n semaphore objects

3. initialize semaphore with:
   sem_init(&sem_t semaphore, int pshared, unsigned int initial_value);
      - &sem_t semaphore: address to your semaphore object
      - int pshared: sharing flag; 0 for threads in the same process, 1 for sharing between processes
      - unsigned int initial_value: initial value of the semaphore; sem_wait decrements it (blocks if 0),
      sem_post increments it to signal availability

      for example if we want to declare a binary semaphore you would make:
      sem_init(&semaphore_mutex, 0, 1); <- this will create a semaphore object with initial value of 1 

   example:
   ```
   sem_init(&semaphore, 0, 0) <- this requires the ADDRESS of the semaphore object, pshared=0 for threads, initial_value=0
   ```

   example with semaphore list
   ```
   for (int i = 0; i < length_of_list; i++) {
      sem_init(&list_semaphores[i], 0, 0);
   }
   ```

4. define list of threads
   4.1. pthread_t threads[int n] <-- creates a list of threads 

   4.2. use function int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg);
         to initialize each thread:
            - pthread_t *thread: ptr to the thread u wanna initialize
            - *attr: use NULL for default values
            - *(*start_routine)(void *): put your worker function here
            - *arg: this is the variable u wanna pass to the worker function

      for example you can do:
         pthread_create(&threads[1], NULL, worker1, "Jawa");
      this says "create a thread, associate it with thread idx 1, default settings,
      worker1 contains the logic that we want to run in this thread, pass Jawa to worker1 thread"

   4.3. make main thread wait on other threads to avoid race condition or early stopping:
      We usually make a for loop for n where n is the length of the threads array:
      ```
      for (int i = 0; i<n; i++) {
         // we then use the pthread_join() function 
         pthread_join(threads[i], NULL);
      }

      pthread_join will block the main thread from progressing until that thread is done.

   5. do sem_destroy(&sem_t semaphore);
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>

int num_workers;

// init list of semaphores
sem_t* listsem;

void* worker(void* arg) {
   intptr_t id = (intptr_t) arg;

   // wait for previous thread to finish
   if (id > 0) {
      sem_wait(&listsem[id]);
   }

   printf("thread id: %ld - doing work\n", id + 1);
   srand(time(NULL) ^ pthread_self()); // Seed with time and thread ID for better randomness
   int sleep_time = (rand() % 3) + 1;
   sleep(sleep_time);
   
   printf("thread id: %ld - finished\n", id + 1);

   // signal next thread
   if (id < num_workers - 1) {
      sem_post(&listsem[id + 1]);
   }

   return NULL;
}

int main(void) {
   pthread_t* threadlist;

   printf("Enter the number of workers\n");
   if (scanf("%d", &num_workers) != 1) {
      perror("ERR: scanf failed\n");
      return 1; 
   }

   listsem = malloc(num_workers*sizeof(sem_t));
   threadlist = malloc(num_workers*sizeof(pthread_t));

   // initialize semaphores
   for (int i=0; i<num_workers; i++) {
      sem_init(&listsem[i], 0, num_workers);
      pthread_create(threadlist, NULL, worker, (void*)(intptr_t)i);
   }

   // wait for all threads to finish
   for (int i=0; i<num_workers; i++) {
      pthread_join(threadlist[i], NULL);
   }

   // destroy semaphores
   for (int i=0; i<num_workers; i++) {
      sem_destroy(&listsem[i]);
   }

   free(listsem);
   free(threadlist);

   return 0;
}