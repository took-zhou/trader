/*
 * proxyRecer.cpp
 *      Author: Administrator
 */
#include <map>

#include "common/extern/libgo/libgo/libgo.h"
#include "common/extern/libzmq/include/zhelpers.h"
#include "common/extern/log/log.h"
#include "common/self/utils.h"
#include "trader/infra/recer/proxyRecer.h"
#include "trader/infra/zmqBase.h"

extern MsgStruct NilMsgStruct;
extern std::map<std::string, EventType> TitleToEvent;
extern co_chan<MsgStruct> orderMsgChan;
extern co_chan<MsgStruct> queryMsgChan;

void ProxyRecer::init() {
  INFO_LOG("begin ProxyRecer::init");
  topicList.clear();
  // strategy_trader
  topicList.push_back("strategy_trader.OrderInsertReq");
  topicList.push_back("strategy_trader.AccountStatusReq");
  topicList.push_back("strategy_trader.OrderCancelReq");
  topicList.push_back("strategy_trader.MarginRateReq");
  topicList.push_back("strategy_trader.CommissionRateReq");
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

  queryTopicList.clear();
  orderTopicList.clear();

  queryTopicList.push_back("strategy_trader.AccountStatusReq");
  queryTopicList.push_back("strategy_trader.MarginRateReq");
  queryTopicList.push_back("strategy_trader.CommissionRateReq");
  queryTopicList.push_back("strategy_trader.InstrumentReq");
  queryTopicList.push_back("market_trader.QryInstrumentReq");
  queryTopicList.push_back("manage_trader.AccountStatusReq");
  queryTopicList.push_back("ctpview_trader.LoginControl");
  queryTopicList.push_back("ctpview_trader.BugInjection");

  orderTopicList.push_back("strategy_trader.OrderInsertReq");
  orderTopicList.push_back("strategy_trader.OrderCancelReq");

  auto routine1 = [&]() { query_information_routine(zmqBase.context); };
  std::thread(routine1).detach();

  auto routin2 = [&]() { order_routine(zmqBase.context); };
  std::thread(routin2).detach();

  INFO_LOG("init proxyRecer ok");
}

bool ProxyRecer::checkSessionAndTitle(std::vector<std::string> &sessionAndTitle) { return true; }

bool ProxyRecer::isTopicInSubTopics(std::string title) {
  for (auto &topic : topicList) {
    if (topic == title) {
      return true;
    }
  }
  return false;
}

void *ProxyRecer::query_information_routine(void *context) {
  MsgStruct msg;

  void *receiver = zmq_socket(context, ZMQ_SUB);
  zmq_connect(receiver, "inproc://workers");
  if (receiver == nullptr) {
    ERROR_LOG("receiver is nullptr");
    return NULL;
  }

  for (auto &topic : queryTopicList) {
    zmq_setsockopt(receiver, ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  }

  while (1) {
    char *recContent = s_recv(receiver);
    std::string content = std::string(recContent);
    auto spacePos = content.find_first_of(" ");
    auto title = content.substr(0, spacePos);
    auto pbMsg = content.substr(spacePos + 1);

    // INFO_LOG("recv msg, topic is[%s]",title.c_str());
    std::string tmpEventName = std::string(title);
    std::vector<std::string> sessionAndTitle = utils::splitString(tmpEventName, std::string("."));
    if (sessionAndTitle.size() != 2) {
      ERROR_LOG("receiver content is error!");
      return NULL;
    }
    std::string session = sessionAndTitle.at(0);
    std::string msgTitle = sessionAndTitle.at(1);

    msg.sessionName = session;
    msg.msgName = msgTitle;
    msg.pbMsg = pbMsg;

    queryMsgChan << msg;
  }
  return NULL;
}

void *ProxyRecer::order_routine(void *context) {
  MsgStruct msg;

  void *receiver = zmq_socket(context, ZMQ_SUB);
  zmq_connect(receiver, "inproc://workers");
  if (receiver == nullptr) {
    ERROR_LOG("receiver is nullptr");
    return NULL;
  }

  for (auto &topic : orderTopicList) {
    zmq_setsockopt(receiver, ZMQ_SUBSCRIBE, topic.c_str(), strlen(topic.c_str()));
  }

  while (1) {
    char *recContent = s_recv(receiver);
    std::string content = std::string(recContent);
    auto spacePos = content.find_first_of(" ");
    auto title = content.substr(0, spacePos);
    auto pbMsg = content.substr(spacePos + 1);

    // INFO_LOG("recv msg, topic is[%s]",title.c_str());
    std::string tmpEventName = std::string(title);
    std::vector<std::string> sessionAndTitle = utils::splitString(tmpEventName, std::string("."));
    if (sessionAndTitle.size() != 2) {
      ERROR_LOG("receiver content is error!");
      return NULL;
    }
    std::string session = sessionAndTitle.at(0);
    std::string msgTitle = sessionAndTitle.at(1);

    msg.sessionName = session;
    msg.msgName = msgTitle;
    msg.pbMsg = pbMsg;

    orderMsgChan << msg;
  }
  return NULL;
}
