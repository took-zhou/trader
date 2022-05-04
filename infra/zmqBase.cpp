/*
 * zmqBase.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include <string>
#include <sstream>
#include <unistd.h>

#include "trader/infra/zmqBase.h"
#include "common/self/fileUtil.h"
#include "common/self/utils.h"
#include "common/extern/libzmq/include/zhelpers.h"
#include "common/extern/log/log.h"
#include "common/self/basetype.h"

using json = nlohmann::json;

constexpr U32 WAITTIME_FOR_ZMQ_INIT = 1;

bool ZmqBase::init()
{
    context = zmq_ctx_new();
    receiver = zmq_socket(context, ZMQ_SUB);
    publisher = zmq_socket(context, ZMQ_PUB);

    string local_ip;
    utils::get_local_ip(local_ip);
    string sub_ipaddport = "tcp://" + local_ip + ":8100";
    int result = zmq_connect(receiver, sub_ipaddport.c_str());
    sleep(WAITTIME_FOR_ZMQ_INIT);
    if (result != 0)
    {
        ERROR_LOG("receiver connect to %s failed", sub_ipaddport.c_str());
        return false;
    }

    string pub_ipaddport = "tcp://" + local_ip + ":5556";
    result = zmq_connect(publisher, pub_ipaddport.c_str());
    if (result != 0)
    {
        ERROR_LOG("publisher connect to %s failed", pub_ipaddport.c_str());
        return false;
    }

    workers = zmq_socket(context, ZMQ_PUB);
    result = zmq_bind(workers, "inproc://workers");
    if (result != 0)
    {
        ERROR_LOG("workers bind inproc://workers failed");
        return false;
    }

    INFO_LOG("zmq init ok");
    return true;
}


void ZmqBase::SubscribeTopic(const char* topicStr)
{
    zmq_setsockopt(receiver, ZMQ_SUBSCRIBE, topicStr, strlen(topicStr));
    INFO_LOG("sub topic [%s] ok",topicStr);
}

void ZmqBase::unSubscribeTopic(const char* topicStr)
{
    zmq_setsockopt(receiver, ZMQ_UNSUBSCRIBE, topicStr, strlen(topicStr));
}

int ZmqBase::publishMsg(const char* head, const char* msg)
{
    std::stringstream  tmpStr;
    tmpStr << head << " " << msg;

    int ret = s_send(publisher,  tmpStr.str().c_str());
    if(ret == -1)
    {
        ERROR_LOG("zmq send msg error ret = [%d]",ret);
        return false;
    }

    return true;
}
