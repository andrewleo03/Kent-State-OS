//
// Created by Andrew on 11/1/2024.
//

#include "proj1.h"

int main ()
{
    //Create a shared memory segment using shmget and the fixed key
    //The IPC_CREAT flag and 0666 sets the permissions of the shared memory block
    int ID = shmget(SHMKEY, sizeof(struct shared_buf), IPC_CREAT | 0666);

    //Get the address of the shared memory space and case it to a shared_buffer pointer
    struct shared_buf* sharedTable = (struct shared_buf *) shmat(ID, NULL, 0);

    //Initialize the semaphores and initialize the indexes to 0
    sem_init(&sharedTable -> mutex, 1, 1);
    sem_init(&sharedTable -> produce, 1, BUFFER_SIZE);
    sem_init(&sharedTable -> consume, 1, 0);
    sharedTable -> produce_index = 0;
    sharedTable -> consume_index = 0;

    for (int i = 0; i < 10; i++)
    {
        sem_wait(&sharedTable -> produce); // Wait until consumer is ready
        sem_wait(&sharedTable -> mutex);

        sharedTable->table[sharedTable->produce_index] = i; //Put a new value in the tale
        printf("Producer added: %d at index %d\n", i, sharedTable->produce_index);
        sharedTable->produce_index = (sharedTable->produce_index + 1) % BUFFER_SIZE; //Increment index

        sem_post(&sharedTable -> mutex);
        sem_post(&sharedTable -> consume); // Signal consumer
        sleep(1);
    }

    return 0;
}