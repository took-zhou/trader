/*
 * proxySender.cpp
 *
 *  Created on: 2020年08月29日
 *      Author: Administrator
 */

#include "trader/infra/sender/proxySender.h"
#include "trader/infra/zmqBase.h"

bool ProxySender::send(const char *head, const char *msg) {
  auto &zmqBase = ZmqBase::getInstance();
  return zmqBase.SendMsg(head, msg);
}
