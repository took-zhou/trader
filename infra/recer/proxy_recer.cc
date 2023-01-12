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
  InitQueryReceiver();
  InitOrderReceiver();

  SubscribeQueryTopic();
  SubscribeOrderTopic();
}

bool ProxyRecer::InitQueryReceiver(void) {
  auto &zmq_base = BaseZmq::GetInstance();
  query_receiver_ = zmq_socket(zmq_base.context, ZMQ_SUB);
  string sub_ipaddport = "tcp://" + zmq_base.local_ip + ":8100";
  int result = zmq_connect(query_receiver_, sub_ipaddport.c_str());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  if (result != 0) {
    ERROR_LOG("query_receiver_ bind to %s failed", sub_ipaddport.c_str());
  } else {
    INFO_LOG("query_receiver_ bind to %s ok", sub_ipaddport.c_str());
  }

  return true;
}

bool ProxyRecer::InitOrderReceiver(void) {
  auto &zmq_base = BaseZmq::GetInstance();
  order_receiver_ = zmq_socket(zmq_base.context, ZMQ_SUB);
  string sub_ipaddport = "tcp://" + zmq_base.local_ip + ":8100";
  int result = zmq_connect(order_receiver_, sub_ipaddport.c_str());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  if (result != 0) {
    ERROR_LOG("order_receiver_ bind to %s failed", sub_ipaddport.c_str());
  } else {
    INFO_LOG("order_receiver_ bind to %s ok", sub_ipaddport.c_str());
  }

  return true;
}

bool ProxyRecer::IsTopicInQuerySubTopics(std::string title) {
  for (auto &topic : query_topic_list) {
    if (topic == title) {
      return true;
    }
  }
  return false;
}

bool ProxyRecer::IsTopicInOrderSubTopics(std::string title) {
  for (auto &topic : order_topic_list) {
    if (topic == title) {
      return true;
    }
  }
  return false;
}

void ProxyRecer::SubscribeQueryTopic() {
  query_topic_list.clear();

  query_topic_list.push_back("strategy_trader.ActiveSafetyRsp");
  query_topic_list.push_back("strategy_trader.TransactionCostReq");
  query_topic_list.push_back("strategy_trader.AccountStatusReq");
  query_topic_list.push_back("market_trader.QryInstrumentReq");
  query_topic_list.push_back("ctpview_trader.LoginControl");
  query_topic_list.push_back("ctpview_trader.BugInjection");
  query_topic_list.push_back("ctpview_trader.ProfilerControl");

  for (auto &topic : query_topic_list) {
    zmq_setsockopt(query_receiver_, ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  }
}

void ProxyRecer::UnSubscribeQueryTopic() {}

void ProxyRecer::SubscribeOrderTopic() {
  order_topic_list.clear();

  order_topic_list.push_back("strategy_trader.OrderInsertReq");
  order_topic_list.push_back("strategy_trader.OrderCancelReq");

  for (auto &topic : order_topic_list) {
    zmq_setsockopt(order_receiver_, ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  }
}

void ProxyRecer::UnSubscribeOrderTopic() {}

bool ProxyRecer::ReceQueryMsg(utils::ItpMsg &msg) {
  std::string recv_string;
  recv_string.resize(256);

  int msgsize = zmq_recv(query_receiver_, &recv_string[0], recv_string.length() - 1, 0);
  return utils::ReceMsg(msg, recv_string, msgsize);
}

bool ProxyRecer::ReceOrderMsg(utils::ItpMsg &msg) {
  std::string recv_string;
  recv_string.resize(256);

  int msgsize = zmq_recv(order_receiver_, &recv_string[0], recv_string.length() - 1, 0);
  return utils::ReceMsg(msg, recv_string, msgsize);
}
