/*
 * proxySender.h
 *
 *  Created on: 2020年08月29日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_PROXYSENDER_H_
#define WORKSPACE_TRADER_INFRA_PROXYSENDER_H_


#include "trader/infra/define.h"

struct ZmqBase;

struct ProxySender
{
    bool init();
    bool send(const char* head, const char* msg);
    ZmqBase* zmq{nullptr};
};


#endif /* WORKSPACE_TRADER_INFRA_PROXYSENDER_H_ */
