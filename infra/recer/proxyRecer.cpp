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

char *ProxyRecer::queryRecv() {
  enum { cap = 256 };
  char buffer[cap];
  int size = zmq_recv(query_receiver, buffer, cap - 1, 0);
  if (size == -1) return NULL;
  buffer[size < cap ? size : cap - 1] = '\0';

  return strndup(buffer, sizeof(buffer) - 1);
}

char *ProxyRecer::orderRecv() {
  enum { cap = 256 };
  char buffer[cap];
  int size = zmq_recv(order_receiver, buffer, cap - 1, 0);
  if (size == -1) return NULL;
  buffer[size < cap ? size : cap - 1] = '\0';

  return strndup(buffer, sizeof(buffer) - 1);
}

bool ProxyRecer::receQueryMsg(utils::ItpMsg &msg) {
  bool out = true;

  // INFO_LOG("prepare recv titleChar");
  char *recContent = queryRecv();
  if (recContent != nullptr) {
    int index = 0;
    int segIndex = 0;
    int length = strlen(recContent) + 1;
    char temp[length];
    for (int i = 0; i < length; i++) {
      temp[index] = recContent[i];
      if (recContent[i] == '.' && segIndex == 0) {
        temp[index] = '\0';
        msg.sessionName = temp;
        index = 0;
        segIndex++;
      } else if (recContent[i] == ' ' && segIndex == 1) {
        temp[index] = '\0';
        msg.msgName = temp;
        index = 0;
        segIndex++;
      } else if (recContent[i] == '\0' && segIndex == 2) {
        msg.pbMsg = temp;
        break;
      } else {
        index++;
      }
    }
  } else {
    out = false;
  }

  return out;
}

bool ProxyRecer::receOrderMsg(utils::ItpMsg &msg) {
  bool out = true;

  // INFO_LOG("prepare recv titleChar");
  char *recContent = orderRecv();
  if (recContent != nullptr) {
    int index = 0;
    int segIndex = 0;
    int length = strlen(recContent) + 1;
    char temp[length];
    for (int i = 0; i < length; i++) {
      temp[index] = recContent[i];
      if (recContent[i] == '.' && segIndex == 0) {
        temp[index] = '\0';
        msg.sessionName = temp;
        index = 0;
        segIndex++;
      } else if (recContent[i] == ' ' && segIndex == 1) {
        temp[index] = '\0';
        msg.msgName = temp;
        index = 0;
        segIndex++;
      } else if (recContent[i] == '\0' && segIndex == 2) {
        msg.pbMsg = temp;
        break;
      } else {
        index++;
      }
    }
  } else {
    out = false;
  }

  return out;
}
