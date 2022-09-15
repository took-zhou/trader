/*
 * proxySender.cpp
 *
 *  Created on: 2020年08月29日
 *      Author: Administrator
 */

#include "trader/infra/sender/proxy_sender.h"
#include "trader/infra/base_zmq.h"

bool ProxySender::Send(utils::ItpMsg &msg) {
  auto &zmq_base = BaseZmq::GetInstance();
  std::string outstring = msg.session_name + "." + msg.msg_name + " " + msg.pb_msg;
  return zmq_base.SendMsg(outstring);
}