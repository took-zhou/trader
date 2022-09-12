/*
 * proxySender.cpp
 *
 *  Created on: 2020年08月29日
 *      Author: Administrator
 */

#include "trader/infra/sender/proxy_sender.h"
#include "trader/infra/base_zmq.h"

bool ProxySender::Send(utils::ItpMsg &msg) {
  auto &zmqBase = BaseZmq::getInstance();
  std::string outstring = msg.sessionName + "." + msg.msgName + " " + msg.pbMsg;
  return zmqBase.SendMsg(outstring);
}