/*
 * directRecer.cpp
 *      Author: Administrator
 */
#include "trader/infra/recer/direct_recer.h"
#include <map>
#include <thread>
#include "common/extern/libzmq/include/zmq.h"
#include "common/extern/log/log.h"
#include "common/self/utils.h"
#include "trader/infra/base_zmq.h"

DirectRecer::DirectRecer() {
  auto &zmq_base = BaseZmq::GetInstance();
  receiver_ = zmq_socket(zmq_base.GetContext(), ZMQ_SUB);
  string sub_ipaddport = "tcp://" + zmq_base.GetLocalIp() + ":8101";
  int result = zmq_bind(receiver_, sub_ipaddport.c_str());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  if (result != 0) {
    ERROR_LOG("receiver_ bind to %s failed", sub_ipaddport.c_str());
  } else {
    INFO_LOG("receiver_ bind to %s ok", sub_ipaddport.c_str());
  }

  SubscribeTopic();
}

bool DirectRecer::IsTopicInSubTopics(const std::string &title) {
  for (auto &topic : topic_list_) {
    if (topic == title) {
      return true;
    }
  }
  return false;
}

void DirectRecer::SubscribeTopic() { zmq_setsockopt(receiver_, ZMQ_SUBSCRIBE, "", 0); }

void DirectRecer::UnSubscribeTopic() {}

bool DirectRecer::ReceMsg(utils::ItpMsg &msg) {
  std::string recv_string;
  recv_string.resize(256);

  int msgsize = zmq_recv(receiver_, &recv_string[0], recv_string.length() - 1, 0);
  return utils::ReceMsg(msg, recv_string, msgsize);
}
