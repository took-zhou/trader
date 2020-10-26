/*
 * proxySender.cpp
 *
 *  Created on: 2020��8��30��
 *      Author: Administrator
 */

#include "trader/infra/sender/proxySender.h"
#include "trader/infra/zmqBase.h"

bool ProxySender::init()
{
    auto& zmqBase = ZmqBase::getInstance();
    zmq = &zmqBase;

    return true;
}

bool ProxySender::send(const char* head, const char* msg)
{
    auto& zmqBase = ZmqBase::getInstance();
    return zmqBase.publishMsg(head, msg);
}
