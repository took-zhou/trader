/*
 * zmqBase.h
 *
 *  Created on: 2020��9��1��
 *      Author: Administrator
 */

#ifndef WORKSPACE_STRATEGY_INFRA_ZMQBASE_H_
#define WORKSPACE_STRATEGY_INFRA_ZMQBASE_H_

#include "common/extern/libzmq/include/zmq.hpp"
#include <string>
#include "common/self/getconfig.h"

struct ZmqBase
{
    ZmqBase(){}
    ZmqBase(const ZmqBase&) = delete;
    ZmqBase& operator=(const ZmqBase&) = delete;
    static ZmqBase& getInstance()
    {
        static ZmqBase instance;
        return instance;
    }
    bool init();
    void SubscribeTopic(const char* topicStr);
    void unSubscribeTopic(const char* topicStr);
    int publishMsg(const char* head, const char* msg);
    void* context  {nullptr};
    void* receiver {nullptr};
    void* publisher{nullptr};
};



#endif /* WORKSPACE_STRATEGY_INFRA_ZMQBASE_H_ */
