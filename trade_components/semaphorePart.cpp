#include "semaphorePart.h"
#include "log.h"
GlobalSem globalSem;


bool GlobalSem::addOrderSem(string orderRef)
{
    orderSem* newSem = new orderSem;
    newSem->orderRef = orderRef;
    orderSems.push_back(newSem);
    return true;
}

bool GlobalSem::delOrderSem(string orderRef)
{
    if(orderSems.size() == 0)
    {
        ERROR_LOG("del sem error, ordersems size = 0");
        return false;
    }
    for(unsigned int i = 0; i< orderSems.size(); i++)
    {
        auto tmpSem = orderSems.at(i);
        if(tmpSem->orderRef == orderRef)
        {
            delete tmpSem;
            tmpSem = nullptr;
            orderSems.erase(orderSems.begin()+i);
            return true;
        }
        continue;
    }
    ERROR_LOG("can't find the sem with orderRef:%s",orderRef.c_str());
    return false;
}

void GlobalSem::waitSemByOrderRef(string orderRef)
{
    for(unsigned int i = 0; i< orderSems.size(); i++)
    {
        auto tmpSem = orderSems.at(i);
        if(tmpSem->orderRef == orderRef)
        {
            sem_wait(&(tmpSem->sem));
            return;
        }
        continue;
    }
}

void GlobalSem::postSemByOrderRef(string orderRef)
{
    for(unsigned int i = 0; i< orderSems.size(); i++)
    {
        auto tmpSem = orderSems.at(i);
        if(tmpSem->orderRef == orderRef)
        {
            sem_post(&(tmpSem->sem));
            return;
        }
        continue;
    }
}
