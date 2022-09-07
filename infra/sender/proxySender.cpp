/*
 * proxySender.cpp
 *
 *  Created on: 2020年08月29日
 *      Author: Administrator
 */

#include "trader/infra/sender/proxySender.h"
#include "trader/infra/zmqBase.h"

bool ProxySender::send(utils::ItpMsg &msg) {
  auto &zmqBase = ZmqBase::getInstance();
  std::string outstring = msg.sessionName + "." + msg.msgName + " " + msg.pbMsg;
  return zmqBase.SendMsg(outstring);
}