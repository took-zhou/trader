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
  topic_list.push_back("strategy_trader.AccountStatusReq");
  topic_list.push_back("strategy_trader.OrderCancelReq");
  topic_list.push_back("strategy_trader.TransactionCostReq");
  topic_list.push_back("strategy_trader.InstrumentReq");
  // trader_trader
  topic_list.push_back("trader_trader.HeartBeat");

  // market_trader
  topic_list.push_back("market_trader.QryInstrumentReq");

  // manage_trader
  topic_list.push_back("manage_trader.AccountStatusReq");

  topic_list.push_back("ctpview_trader.LoginControl");
  topic_list.push_back("ctpview_trader.BugInjection");

  auto &zmqBase = BaseZmq::getInstance();
  for (auto &topic : topic_list) {
    zmqBase.SubscribeTopic(topic.c_str());
  }

  InitQueryReceiver();
  InitOrderReceiver();

  INFO_LOG("init proxyRecer ok");
}

bool ProxyRecer::InitQueryReceiver(void) {
  query_topic_list.clear();

  query_topic_list.push_back("strategy_trader.AccountStatusReq");
  query_topic_list.push_back("strategy_trader.TransactionCostReq");
  query_topic_list.push_back("strategy_trader.InstrumentReq");
  query_topic_list.push_back("market_trader.QryInstrumentReq");
  query_topic_list.push_back("manage_trader.AccountStatusReq");
  query_topic_list.push_back("ctpview_trader.LoginControl");
  query_topic_list.push_back("ctpview_trader.BugInjection");

  auto &zmqBase = BaseZmq::getInstance();
  query_receiver = zmq_socket(zmqBase.context, ZMQ_SUB);
  zmq_connect(query_receiver, "inproc://workers");
  if (query_receiver == nullptr) {
    ERROR_LOG("query_receiver is nullptr");
  }

  for (auto &topic : query_topic_list) {
    zmq_setsockopt(query_receiver, ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  }
}

bool ProxyRecer::InitOrderReceiver(void) {
  order_topic_list.clear();

  order_topic_list.push_back("strategy_trader.OrderInsertReq");
  order_topic_list.push_back("strategy_trader.OrderCancelReq");

  auto &zmqBase = BaseZmq::getInstance();
  order_receiver = zmq_socket(zmqBase.context, ZMQ_SUB);
  zmq_connect(order_receiver, "inproc://workers");
  if (order_receiver == nullptr) {
    ERROR_LOG("order_receiver is nullptr");
  }

  for (auto &topic : order_topic_list) {
    zmq_setsockopt(order_receiver, ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  }
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

  std::string recvString;
  recvString.resize(256);

  int msgsize = zmq_recv(query_receiver, &recvString[0], recvString.length() - 1, 0);
  if (msgsize != -1) {
    int startIndex = 0;
    int segIndex = 0;

    for (int i = 0; i < msgsize; i++) {
      if (recvString[i] == '.') {
        if (segIndex == 0) {
          msg.sessionName.resize(i - startIndex);
          memcpy(&msg.sessionName[0], &recvString[startIndex], (i - startIndex));
        } else if (segIndex == 1) {
          msg.msgName.resize(i - startIndex);
          memcpy(&msg.msgName[0], &recvString[startIndex], (i - startIndex));
        }
        startIndex = i + 1;
        segIndex++;
      } else if (recvString[i] == ' ') {
        if (segIndex == 1) {
          i = i;
          msg.msgName.resize(i - startIndex);
          memcpy(&msg.msgName[0], &recvString[startIndex], (i - startIndex));
        }
        startIndex = i + 1;
        break;
      }
    }

    msg.pbMsg.resize(msgsize - startIndex);
    memcpy(&msg.pbMsg[0], &recvString[startIndex], (msgsize - startIndex));
  } else {
    out = false;
  }
  return out;
}

bool ProxyRecer::ReceOrderMsg(utils::ItpMsg &msg) {
  bool out = true;

  std::string recvString;
  recvString.resize(256);

  int msgsize = zmq_recv(order_receiver, &recvString[0], recvString.length() - 1, 0);
  if (msgsize != -1) {
    int startIndex = 0;
    int segIndex = 0;

    for (int i = 0; i < msgsize; i++) {
      if (recvString[i] == '.') {
        if (segIndex == 0) {
          msg.sessionName.resize(i - startIndex);
          memcpy(&msg.sessionName[0], &recvString[startIndex], (i - startIndex));
        } else if (segIndex == 1) {
          msg.msgName.resize(i - startIndex);
          memcpy(&msg.msgName[0], &recvString[startIndex], (i - startIndex));
        }
        startIndex = i + 1;
        segIndex++;
      } else if (recvString[i] == ' ') {
        if (segIndex == 1) {
          i = i;
          msg.msgName.resize(i - startIndex);
          memcpy(&msg.msgName[0], &recvString[startIndex], (i - startIndex));
        }
        startIndex = i + 1;
        break;
      }
    }

    msg.pbMsg.resize(msgsize - startIndex);
    memcpy(&msg.pbMsg[0], &recvString[startIndex], (msgsize - startIndex));
  } else {
    out = false;
  }

  return out;
}
