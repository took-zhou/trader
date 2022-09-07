/*
 * proxyRecer.cpp
 *      Author: Administrator
 */
#include "trader/infra/recer/proxyRecer.h"
#include <map>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/utils.h"
#include "trader/infra/zmqBase.h"

ProxyRecer::ProxyRecer() {
  INFO_LOG("begin ProxyRecer::init");
  topicList.clear();
  // strategy_trader
  topicList.push_back("strategy_trader.OrderInsertReq");
  topicList.push_back("strategy_trader.AccountStatusReq");
  topicList.push_back("strategy_trader.OrderCancelReq");
  topicList.push_back("strategy_trader.TransactionCostReq");
  topicList.push_back("strategy_trader.InstrumentReq");
  // trader_trader
  topicList.push_back("trader_trader.HeartBeat");

  // market_trader
  topicList.push_back("market_trader.QryInstrumentReq");

  // manage_trader
  topicList.push_back("manage_trader.AccountStatusReq");

  topicList.push_back("ctpview_trader.LoginControl");
  topicList.push_back("ctpview_trader.BugInjection");

  auto &zmqBase = ZmqBase::getInstance();
  for (auto &topic : topicList) {
    zmqBase.SubscribeTopic(topic.c_str());
  }

  initQueryReceiver();
  initOrderReceiver();

  INFO_LOG("init proxyRecer ok");
}

bool ProxyRecer::initQueryReceiver(void) {
  queryTopicList.clear();

  queryTopicList.push_back("strategy_trader.AccountStatusReq");
  queryTopicList.push_back("strategy_trader.TransactionCostReq");
  queryTopicList.push_back("strategy_trader.InstrumentReq");
  queryTopicList.push_back("market_trader.QryInstrumentReq");
  queryTopicList.push_back("manage_trader.AccountStatusReq");
  queryTopicList.push_back("ctpview_trader.LoginControl");
  queryTopicList.push_back("ctpview_trader.BugInjection");

  auto &zmqBase = ZmqBase::getInstance();
  query_receiver = zmq_socket(zmqBase.context, ZMQ_SUB);
  zmq_connect(query_receiver, "inproc://workers");
  if (query_receiver == nullptr) {
    ERROR_LOG("query_receiver is nullptr");
  }

  for (auto &topic : queryTopicList) {
    zmq_setsockopt(query_receiver, ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  }
}

bool ProxyRecer::initOrderReceiver(void) {
  orderTopicList.clear();

  orderTopicList.push_back("strategy_trader.OrderInsertReq");
  orderTopicList.push_back("strategy_trader.OrderCancelReq");

  auto &zmqBase = ZmqBase::getInstance();
  order_receiver = zmq_socket(zmqBase.context, ZMQ_SUB);
  zmq_connect(order_receiver, "inproc://workers");
  if (order_receiver == nullptr) {
    ERROR_LOG("order_receiver is nullptr");
  }

  for (auto &topic : orderTopicList) {
    zmq_setsockopt(order_receiver, ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  }
}

bool ProxyRecer::isTopicInSubTopics(std::string title) {
  for (auto &topic : topicList) {
    if (topic == title) {
      return true;
    }
  }
  return false;
}

bool ProxyRecer::receQueryMsg(utils::ItpMsg &msg) {
  bool out = true;

  std::string recvString;
  recvString.resize(256);

  int msgsize = zmq_recv(query_receiver, &recvString[0], recvString.length() - 1, 0);
  if (msgsize != -1) {
    int index = 0;
    int segIndex = 0;
    char temp[msgsize];
    for (int i = 0; i < msgsize; i++) {
      temp[index] = recvString[i];
      if (temp[index] == '.') {
        if (segIndex == 0) {
          temp[index] = '\0';
          msg.sessionName = temp;
        } else if (segIndex == 1) {
          temp[index] = '\0';
          msg.msgName = temp;
        }
        segIndex++;
        index = 0;
      } else if (temp[index] == ' ') {
        if (segIndex == 1) {
          temp[index] = '\0';
          msg.msgName = temp;
        }
        segIndex++;
        index = 0;
      } else {
        index++;
      }
    }
    msg.pbMsg.resize(index);
    memcpy(&msg.pbMsg[0], temp, index);
  } else {
    out = false;
  }
  return out;
}

bool ProxyRecer::receOrderMsg(utils::ItpMsg &msg) {
  bool out = true;

  std::string recvString;
  recvString.resize(256);

  int msgsize = zmq_recv(order_receiver, &recvString[0], recvString.length() - 1, 0);
  if (msgsize != -1) {
    int index = 0;
    int segIndex = 0;

    char temp[msgsize];
    for (int i = 0; i < msgsize; i++) {
      temp[index] = recvString[i];
      if (temp[index] == '.') {
        if (segIndex == 0) {
          temp[index] = '\0';
          msg.sessionName = temp;
        } else if (segIndex == 1) {
          temp[index] = '\0';
          msg.msgName = temp;
        }
        segIndex++;
        index = 0;
      } else if (temp[index] == ' ') {
        if (segIndex == 1) {
          temp[index] = '\0';
          msg.msgName = temp;
        }
        segIndex++;
        index = 0;
      } else {
        index++;
      }
    }
  } else {
    out = false;
  }

  return out;
}
