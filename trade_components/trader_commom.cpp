#include "trader_commom.h"
#include <ctime>
#include "log.h"



//void settimer(unsigned int id, int msec, BACKCALLFUNC backcallfunc)//方式一实现定时器
//{
//
//    if (msec < 0) //判断时间段是否满足 >=0
//    {
//        return;
//    }
//    INFO_LOG("start timer [%d]",id);
//    clock_t start, finish;
//    start = clock();//计时函数
//    double totaltime = 0;//定义时间变量
//    while (1)
//    {
//        finish = clock();
//        totaltime = (double)(finish - start);
//        if (totaltime > msec)
//        {
//            backcallfunc(&totaltime);
////            break;
//        }
//    }
//}


