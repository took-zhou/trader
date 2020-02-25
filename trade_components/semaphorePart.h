#ifndef SEMAPHOREPART_H
#define SEMAPHOREPART_H
#include <semaphore.h>
#include <string>
#include <vector>

using namespace std;
struct orderSem
{
    orderSem()
    {
        sem_init(&sem, 0, 0);
    }
    string orderRef{""};
    sem_t sem;
};


struct GlobalSem
{
    GlobalSem(){
        sem_init(&sem, 0, 0); // @suppress("Function cannot be resolved")
        sem_init(&sem_login, 0, 0);
        sem_init(&sem_logout, 0, 0);
        sem_init(&sem_query, 0, 0);
    }

    bool addOrderSem(string orderRef);
    bool delOrderSem(string orderRef);
    void waitSemByOrderRef(string orderRef);
    void postSemByOrderRef(string orderRef);
public:
    sem_t sem;
    sem_t sem_login;
    sem_t sem_logout;
    sem_t sem_query;

    orderSem semOrder1;
    orderSem semOrder2;

    vector<orderSem*> orderSems;

};


#endif
