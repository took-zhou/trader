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
    receiver = zmq_socket (context, ZMQ_PULL);
    zmq_bind(receiver, inprocAddress.c_str());

    sender = zmq_socket (context, ZMQ_PUSH);
    zmq_connect(sender, inprocAddress.c_str());
    return true;
}

void InnerZmq::pushTask(const std::string& msg)
{
    s_send(sender, msg.c_str());
    INFO_LOG("inner zmq push task ok");
}




