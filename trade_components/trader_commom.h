#ifndef TRADER_COMMON_H
#define TRADER_COMMON_H
#include <stddef.h>
struct Trader_Info
{
    size_t RequestID{0};
};

typedef void(*BACKCALLFUNC)(void *);
void settimer(unsigned int id, int msec, BACKCALLFUNC backcallfunc);//方式一实现定时器

#endif
