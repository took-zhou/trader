/*
 * innerZmq.cpp
 *
 *  Created on: 2021年2月17日
 *      Author: Administrator
 */

#include "trader/infra/innerZmq.h"
#include "common/extern/libzmq/include/zhelpers.h"
#include <cstring>
#include "common/extern/log/log.h"
bool InnerZmq::init()
{
    context = zmq_ctx_new();
    receiver = zmq_socket (context, ZMQ_REP);
    zmq_bind(receiver, inprocAddress.c_str());
    return true;
}

void* InnerZmq::getNewClient()
{
    if(context != nullptr)
    {
        return nullptr;
    }
    void* client = zmq_socket (context, ZMQ_DEALER);
    zmq_connect(client, inprocAddress.c_str());
    return client;
}

int InnerZmq::closeClient(void* client)
{
    return zmq_close(client);
}

void InnerZmq::pushTask(void* client, void* msg, size_t size)
{
    char bytesMsg[size];
    std::memcpy(bytesMsg, (const void*)&msg, size);
    s_send(client, bytesMsg);
    INFO_LOG("inner zmq push task ok");
}



