/*
 * innerZmq.h
 *
 *  Created on: 2021年2月17日
 *      Author: Administrator
 */

#ifndef H56F09BAA_8284_4208_94EE_31E9DC18CE89
#define H56F09BAA_8284_4208_94EE_31E9DC18CE89
#include <string>
#include "trader/infra/define.h"

struct InnerZmq
{
    InnerZmq(){}
    InnerZmq(const InnerZmq&) = delete;
    InnerZmq& operator=(const InnerZmq&) = delete;
    static InnerZmq& getInstance()
    {
        static InnerZmq instance;
        return instance;
    }

    bool init();
    void pushTask(const std::string& msg);
    std::string inprocAddress{"inproc://innerzmq"};
    void* context   {nullptr};
    void* receiver  {nullptr};
    void* sender    {nullptr};
};



#endif /* H56F09BAA_8284_4208_94EE_31E9DC18CE89 */
