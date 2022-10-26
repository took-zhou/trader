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
  INFO_LOG("begin ProxyRecer::init");
  topic_list.clear();
  // strategy_trader
  topic_list.push_back("strategy_trader.OrderInsertReq");
  topic_list.push_back("strategy_trader.SubAccountStatus");
  topic_list.push_back("strategy_trader.UnsubAccountStatus");
  topic_list.push_back("strategy_trader.OrderCancelReq");
  topic_list.push_back("strategy_trader.TransactionCostReq");
  topic_list.push_back("strategy_trader.ActiveSafetyRsp");
  // trader_trader
  topic_list.push_back("trader_trader.HeartBeat");

  // market_trader
  topic_list.push_back("market_trader.QryInstrumentReq");

  topic_list.push_back("ctpview_trader.LoginControl");
  topic_list.push_back("ctpview_trader.BugInjection");
  topic_list.push_back("ctpview_trader.CheckStrategyAlive");

  auto &zmq_base = BaseZmq::GetInstance();
  for (auto &topic : topic_list) {
    zmq_base.SubscribeTopic(topic.c_str());
  }

  InitQueryReceiver();
  InitOrderReceiver();

  INFO_LOG("init ProxyRecer ok");
}

bool ProxyRecer::InitQueryReceiver(void) {
  query_topic_list.clear();

  query_topic_list.push_back("strategy_trader.ActiveSafetyRsp");
  query_topic_list.push_back("strategy_trader.SubAccountStatus");
  query_topic_list.push_back("strategy_trader.UnsubAccountStatus");
  query_topic_list.push_back("strategy_trader.TransactionCostReq");
  query_topic_list.push_back("market_trader.QryInstrumentReq");
  query_topic_list.push_back("ctpview_trader.LoginControl");
  query_topic_list.push_back("ctpview_trader.BugInjection");
  query_topic_list.push_back("ctpview_trader.CheckStrategyAlive");

  auto &zmq_base = BaseZmq::GetInstance();
  query_receiver_ = zmq_socket(zmq_base.context, ZMQ_SUB);
  zmq_connect(query_receiver_, "inproc://workers");
  if (query_receiver_ == nullptr) {
    ERROR_LOG("query_receiver is nullptr");
  }

  for (auto &topic : query_topic_list) {
    zmq_setsockopt(query_receiver_, ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  }

  return true;
}

bool ProxyRecer::InitOrderReceiver(void) {
  order_topic_list.clear();

  order_topic_list.push_back("strategy_trader.OrderInsertReq");
  order_topic_list.push_back("strategy_trader.OrderCancelReq");

  auto &zmq_base = BaseZmq::GetInstance();
  order_receiver_ = zmq_socket(zmq_base.context, ZMQ_SUB);
  zmq_connect(order_receiver_, "inproc://workers");
  if (order_receiver_ == nullptr) {
    ERROR_LOG("order_receiver is nullptr");
  }

  for (auto &topic : order_topic_list) {
    zmq_setsockopt(order_receiver_, ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  }

  return true;
}

bool ProxyRecer::IsTopicInSubTopics(std::string title) {
  for (auto &topic : topic_list) {
    if (topic == title) {
      return true;
    }
  }
  return false;
}

bool ProxyRecer::ReceQueryMsg(utils::ItpMsg &msg) {
  bool out = true;

  std::string recv_string;
  recv_string.resize(256);

  int msgsize = zmq_recv(query_receiver_, &recv_string[0], recv_string.length() - 1, 0);
  if (msgsize != -1) {
    int start_index = 0;
    int seg_index = 0;

    for (int i = 0; i < msgsize; i++) {
      if (recv_string[i] == '.') {
        if (seg_index == 0) {
          msg.session_name.resize(i - start_index);
          memcpy(&msg.session_name[0], &recv_string[start_index], (i - start_index));
        } else if (seg_index == 1) {
          msg.msg_name.resize(i - start_index);
          memcpy(&msg.msg_name[0], &recv_string[start_index], (i - start_index));
        }
        start_index = i + 1;
        seg_index++;
      } else if (recv_string[i] == ' ') {
        if (seg_index == 1) {
          i = i;
          msg.msg_name.resize(i - start_index);
          memcpy(&msg.msg_name[0], &recv_string[start_index], (i - start_index));
        }
        start_index = i + 1;
        break;
      }
    }

    msg.pb_msg.resize(msgsize - start_index);
    memcpy(&msg.pb_msg[0], &recv_string[start_index], (msgsize - start_index));
  } else {
    out = false;
  }
  return out;
}

bool ProxyRecer::ReceOrderMsg(utils::ItpMsg &msg) {
  bool out = true;

  std::string recv_string;
  recv_string.resize(256);

  int msgsize = zmq_recv(order_receiver_, &recv_string[0], recv_string.length() - 1, 0);
  if (msgsize != -1) {
    int start_index = 0;
    int seg_index = 0;

    for (int i = 0; i < msgsize; i++) {
      if (recv_string[i] == '.') {
        if (seg_index == 0) {
          msg.session_name.resize(i - start_index);
          memcpy(&msg.session_name[0], &recv_string[start_index], (i - start_index));
        } else if (seg_index == 1) {
          msg.msg_name.resize(i - start_index);
          memcpy(&msg.msg_name[0], &recv_string[start_index], (i - start_index));
        }
        start_index = i + 1;
        seg_index++;
      } else if (recv_string[i] == ' ') {
        if (seg_index == 1) {
          i = i;
          msg.msg_name.resize(i - start_index);
          memcpy(&msg.msg_name[0], &recv_string[start_index], (i - start_index));
        }
        start_index = i + 1;
        break;
      }
    }

    msg.pb_msg.resize(msgsize - start_index);
    memcpy(&msg.pb_msg[0], &recv_string[start_index], (msgsize - start_index));
  } else {
    out = false;
  }

  return out;
}
