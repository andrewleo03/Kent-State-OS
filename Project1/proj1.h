//
// Created by Andrew on 11/1/2024.
//

#ifndef KENT_STATE_OS_PROJ1_H
#define KENT_STATE_OS_PROJ1_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/shm.h>

#define BUFFER_SIZE 2
#define SHMKEY 1111

struct shared_buf
{
    int table[BUFFER_SIZE];
    int produce_index;
    int consume_index;
    sem_t mutex;
    sem_t produce;
    sem_t consume;
};

#endif //KENT_STATE_OS_PROJ1_H
