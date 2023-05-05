/*
 * proxyRecer.cpp
 *      Author: Administrator
 */
#include "trader/infra/recer/proxy_recer.h"
#include <map>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/utils.h"
#include "trader/infra/base_zmq.h"

ProxyRecer::ProxyRecer() {
  auto &zmq_base = BaseZmq::GetInstance();
  receiver_ = zmq_socket(zmq_base.context, ZMQ_SUB);
  string sub_ipaddport = "tcp://" + zmq_base.local_ip + ":8100";
  int result = zmq_connect(receiver_, sub_ipaddport.c_str());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  if (result != 0) {
    ERROR_LOG("receiver_ bind to %s failed", sub_ipaddport.c_str());
  } else {
    INFO_LOG("receiver_ bind to %s ok", sub_ipaddport.c_str());
  }

  SubscribeTopic();
}

bool ProxyRecer::IsTopicInSubTopics(std::string title) {
  for (auto &topic : topic_list) {
    if (topic == title) {
      return true;
    }
  }
  return false;
}

void ProxyRecer::SubscribeTopic() {
  topic_list.clear();

  topic_list.push_back("strategy_trader.ActiveSafetyRsp");
  topic_list.push_back("strategy_trader.TransactionCostReq");
  topic_list.push_back("market_trader.QryInstrumentReq");
  topic_list.push_back("ctpview_trader.LoginControl");
  topic_list.push_back("ctpview_trader.BugInjection");
  topic_list.push_back("ctpview_trader.ProfilerControl");
  topic_list.push_back("ctpview_trader.UpdatePara");
  topic_list.push_back("trader_trader.AccountStatusReq");
  topic_list.push_back("trader_trader.SendEmail");

  for (auto &topic : topic_list) {
    zmq_setsockopt(receiver_, ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  }
}

void ProxyRecer::UnSubscribeTopic() {}

bool ProxyRecer::ReceMsg(utils::ItpMsg &msg) {
  std::string recv_string;
  recv_string.resize(256);

  int msgsize = zmq_recv(receiver_, &recv_string[0], recv_string.length() - 1, 0);
  return utils::ReceMsg(msg, recv_string, msgsize);
}
