/*
 * directSender.cpp
 *
 *  Created on: 2020年08月29日
 *      Author: Administrator
 */

#include "trader/infra/sender/direct_sender.h"
#include <thread>
#include "common/extern/libzmq/include/zmq.h"
#include "common/extern/log/log.h"
#include "trader/infra/base_zmq.h"

DirectSender::DirectSender() {
  publisher_ = zmq_socket(BaseZmq::GetInstance().GetContext(), ZMQ_PUB);

  pub_ipaddport_ = "tcp://" + BaseZmq::GetInstance().GetLocalIp() + ":5558";
  int result = zmq_bind(publisher_, pub_ipaddport_.c_str());
  std::this_thread::sleep_for(std::chrono::seconds(1));

  if (result != 0) {
    ERROR_LOG("publisher connect to %s failed", pub_ipaddport_.c_str());
  } else {
    INFO_LOG("publisher connect to %s ok", pub_ipaddport_.c_str());
  }
}

DirectSender::~DirectSender() {
  zmq_close(publisher_);
  INFO_LOG("publisher disconnect to %s ok", pub_ipaddport_.c_str());
}

bool DirectSender::SendMsg(utils::ItpMsg &msg) {
  std::string outstring;
  outstring += msg.session_name;
  outstring += ".";
  outstring += msg.msg_name;
  outstring += " ";
  outstring += msg.pb_msg;
  int size = zmq_send(publisher_, const_cast<char *>(outstring.c_str()), outstring.length(), 0);
  return (size > 0);
}