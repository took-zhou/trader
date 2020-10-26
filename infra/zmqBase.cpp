/*
 * zmqBase.cpp
 *
 *  Created on: 2020��9��1��
 *      Author: Administrator
 */

#include "trader/infra/zmqBase.h"
#include "common/self/fileUtil.h"
#include "common/extern/libzmq/include/zhelpers.h"
#include "common/extern/log/log.h"
using json = nlohmann::json;

bool ZmqBase::init()
{
    context = zmq_ctx_new();
    receiver = zmq_socket(context, ZMQ_SUB);
    publisher = zmq_socket(context, ZMQ_REQ);
    auto& jsonCfg = utils::JsonConfig::getInstance();
    std::string netStr = jsonCfg.getConfig("trader", "SubAddPort").get<std::string>();
    int result = zmq_connect(receiver, netStr.c_str());
    INFO_LOG("result = %d",result);
    if(result != 0)
    {
        ERROR_LOG("receiver connect to %s failed",netStr.c_str());
        return false;
    }
    netStr = jsonCfg.getConfig("trader", "PubAddPort").get<std::string>();

    result = zmq_connect(publisher, netStr.c_str());
    INFO_LOG("result = %d",result);
    if(result != 0)
    {
        ERROR_LOG("publisher connect to %s failed",netStr.c_str());
        return false;
    }
    INFO_LOG("zmq init ok");
    return true;
}


void ZmqBase::SubscribeTopic(const char* topicStr)
{
    zmq_setsockopt(receiver, ZMQ_SUBSCRIBE, topicStr, 1);
}

void ZmqBase::unSubscribeTopic(const char* topicStr)
{
    zmq_setsockopt(receiver, ZMQ_UNSUBSCRIBE, topicStr, 1);
}

int ZmqBase::publishMsg(const char* head, const char* msg)
{
    std::stringstream  tmpStr;
    tmpStr << head << " " << msg;
    WARNING_LOG("tmpStr is %s", tmpStr.str().c_str());

    int ret = s_send(publisher,  tmpStr.str().c_str());
    if(ret == -1)
    {
        ERROR_LOG("zmq send msg error ret = [%d]",ret);
        return false;
    }
    s_recv(publisher);

    return true;
}
