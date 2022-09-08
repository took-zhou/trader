/*
 * traderEvent.cpp
 *
 *  Created on: 2020年9月20日
 *      Author: Administrator
 */

#include "trader/interface/traderEvent.h"
#include <chrono>
#include <thread>
#include "common/self/semaphorePart.h"
#include "trader/domain/traderService.h"
#include "trader/infra/recerSender.h"

TraderEvent::TraderEvent() { regSessionFunc(); }

void TraderEvent::regSessionFunc() {
  int cnt = 0;
  sessionFuncMap.clear();
  sessionFuncMap["market_trader"] = [this](utils::ItpMsg msg) { ROLE(MarketEvent).handle(msg); };
  sessionFuncMap["strategy_trader"] = [this](utils::ItpMsg msg) { ROLE(StrategyEvent).handle(msg); };
  sessionFuncMap["manage_trader"] = [this](utils::ItpMsg msg) { ROLE(ManageEvent).handle(msg); };
  sessionFuncMap["ctp_trader"] = [this](utils::ItpMsg msg) { ROLE(CtpEvent).handle(msg); };
  sessionFuncMap["xtp_trader"] = [this](utils::ItpMsg msg) { ROLE(XtpEvent).handle(msg); };
  sessionFuncMap["ctpview_trader"] = [this](utils::ItpMsg msg) { ROLE(CtpviewEvent).handle(msg); };

  for (auto &iter : sessionFuncMap) {
    INFO_LOG("sessionFuncMap[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

bool TraderEvent::run() {
  auto &recerSender = RecerSender::getInstance();

  auto orderRecRun = [&]() {
    utils::ItpMsg msg;
    while (1) {
      if (recerSender.ROLE(Recer).ROLE(ProxyRecer).receOrderMsg(msg) == false) {
        ERROR_LOG("invalid msg, session is [%s], msgName is [%s]", msg.sessionName.c_str(), msg.msgName.c_str());
        continue;
      }

      INFO_LOG("recv msg, session is[%s], msgName is[%s] ", msg.sessionName.c_str(), msg.msgName.c_str());
      if (sessionFuncMap.find(msg.sessionName) != sessionFuncMap.end()) {
        sessionFuncMap[msg.sessionName](msg);
      } else {
        ERROR_LOG("can not find[%s] in sessionFuncMap", msg.sessionName.c_str());
      }
    }
  };
  INFO_LOG("orderRecRun prepare ok");
  std::thread(orderRecRun).detach();

  auto queryRecRun = [&]() {
    utils::ItpMsg msg;
    while (1) {
      if (recerSender.ROLE(Recer).ROLE(ProxyRecer).receQueryMsg(msg) == false) {
        ERROR_LOG(" invalid msg, session is [%s], msgName is [%s]", msg.sessionName.c_str(), msg.msgName.c_str());
        continue;
      }

      if (sessionFuncMap.find(msg.sessionName) != sessionFuncMap.end()) {
        sessionFuncMap[msg.sessionName](msg);
        std::this_thread::sleep_for(1s);
      } else {
        ERROR_LOG("can not find[%s] in sessionFuncMap", msg.sessionName.c_str());
      }
    }
  };
  INFO_LOG("queryRecRun prepare ok");
  std::thread(queryRecRun).detach();

  auto itpRecRun = [&]() {
    utils::ItpMsg msg;
    while (1) {
      if (recerSender.ROLE(Recer).ROLE(ItpRecer).receMsg(msg) == false) {
        ERROR_LOG(" invalid msg, session is [%s], msgName is [%s]", msg.sessionName.c_str(), msg.msgName.c_str());
        GlobalSem::getInstance().postSemBySemName(GlobalSem::apiRecv);
        continue;
      }

      if (sessionFuncMap.find(msg.sessionName) != sessionFuncMap.end()) {
        sessionFuncMap[msg.sessionName](msg);
      } else {
        ERROR_LOG("can not find[%s] in sessionFuncMap", msg.sessionName.c_str());
      }

      GlobalSem::getInstance().postSemBySemName(GlobalSem::apiRecv);
    }
  };
  INFO_LOG("itpRecRun prepare ok");
  std::thread(itpRecRun).detach();

  recerSender.run();

  return true;
}
