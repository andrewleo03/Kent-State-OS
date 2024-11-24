//
// Created by Andrew on 11/1/2024.
//

#include "proj1.h"

int main ()
{
    //Get the location of the shared memory block since they both use the same fixed key
    int ID = shmget(SHMKEY, sizeof(struct shared_buf), 0666);

    //Get the location of the shared memory using ID and cast it to the shared_buf pointer
    struct shared_buf* sharedTable = (struct shared_buf *)shmat(ID, NULL, 0);

    for (int i = 0; i < 10; i++)
    {
        sem_wait(&sharedTable -> consume); // Wait for producer
        sem_wait(&sharedTable -> mutex);

        int item = sharedTable->table[sharedTable->consume_index]; //Get the value from the table
        printf("Consumer took: %d from index %d\n", item, sharedTable->consume_index);
        sharedTable->consume_index = (sharedTable->consume_index + 1) % BUFFER_SIZE; //Increment the index

        sem_post(&sharedTable -> mutex);
        sem_post(&sharedTable -> produce); // Signal producer
    }
    //Cleanup by detaching shared memory from the process and then removing the segment once the processes finish
    shmdt(sharedTable);
    shmctl(ID, IPC_RMID, NULL);
    return 0;
}
