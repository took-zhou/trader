/*
 * proxySender.cpp
 *
 *  Created on: 2020年08月29日
 *      Author: Administrator
 */

#include "trader/infra/sender/proxy_sender.h"
#include <thread>
#include "common/extern/log/log.h"
#include "trader/infra/base_zmq.h"

ProxySender::ProxySender() {
  publisher_ = zmq_socket(BaseZmq::GetInstance().GetContext(), ZMQ_PUB);

  string pub_ipaddport = "tcp://" + BaseZmq::GetInstance().GetLocalIp() + ":5556";
  int result = zmq_connect(publisher_, pub_ipaddport.c_str());
  std::this_thread::sleep_for(std::chrono::seconds(1));

  if (result != 0) {
    ERROR_LOG("publisher connect to %s failed", pub_ipaddport.c_str());
  } else {
    INFO_LOG("publisher connect to %s ok", pub_ipaddport.c_str());
  }
}

bool ProxySender::SendMsg(utils::ItpMsg &msg) {
  m_lock_.lock();
  std::string outstring;
  outstring += msg.session_name;
  outstring += ".";
  outstring += msg.msg_name;
  outstring += " ";
  outstring += msg.pb_msg;
  int size = zmq_send(publisher_, const_cast<char *>(outstring.c_str()), outstring.length(), 0);
  m_lock_.unlock();
  return (size > 0);
}