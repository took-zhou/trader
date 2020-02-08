#ifndef SEMAPHOREPART_H
#define SEMAPHOREPART_H
#include <semaphore.h>

struct GlobalSem
{
    GlobalSem(){
        sem_init(&sem, 0, 0); // @suppress("Function cannot be resolved")
        sem_init(&sem_login, 0, 0);
        sem_init(&sem_logout, 0, 0);
        sem_init(&sem_query, 0, 0);
    }
    sem_t sem;
    sem_t sem_login;
    sem_t sem_logout;
    sem_t sem_query;
};

#endif
