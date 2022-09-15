/*
 * traderEvent.cpp
 *
 *  Created on: 2020年9月20日
 *      Author: Administrator
 */

#include "trader/interface/trader_event.h"
#include <chrono>
#include <thread>
#include "common/self/semaphore.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

TraderEvent::TraderEvent() { RegSessionFunc(); }

void TraderEvent::RegSessionFunc() {
  int cnt = 0;
  session_func_map.clear();
  session_func_map["market_trader"] = [this](utils::ItpMsg msg) { ROLE(MarketEvent).Handle(msg); };
  session_func_map["strategy_trader"] = [this](utils::ItpMsg msg) { ROLE(StrategyEvent).Handle(msg); };
  session_func_map["manage_trader"] = [this](utils::ItpMsg msg) { ROLE(ManageEvent).Handle(msg); };
  session_func_map["ctp_trader"] = [this](utils::ItpMsg msg) { ROLE(CtpEvent).Handle(msg); };
  session_func_map["xtp_trader"] = [this](utils::ItpMsg msg) { ROLE(XtpEvent).Handle(msg); };
  session_func_map["ctpview_trader"] = [this](utils::ItpMsg msg) { ROLE(CtpviewEvent).Handle(msg); };

  for (auto &iter : session_func_map) {
    INFO_LOG("session_func_map[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

bool TraderEvent::Run() {
  auto &recer_sender = RecerSender::GetInstance();

  auto order_rec_run = [&]() {
    utils::ItpMsg msg;
    while (1) {
      if (recer_sender.ROLE(Recer).ROLE(ProxyRecer).ReceOrderMsg(msg) == false) {
        ERROR_LOG("invalid msg, session is [%s], msg_name is [%s]", msg.session_name.c_str(), msg.msg_name.c_str());
        continue;
      }

      INFO_LOG("recv msg, session is[%s], msg_name is[%s] ", msg.session_name.c_str(), msg.msg_name.c_str());
      if (session_func_map.find(msg.session_name) != session_func_map.end()) {
        session_func_map[msg.session_name](msg);
      } else {
        ERROR_LOG("can not find[%s] in session_func_map", msg.session_name.c_str());
      }
    }
  };
  INFO_LOG("orderRecRun prepare ok");
  std::thread(order_rec_run).detach();

  auto query_rec_run = [&]() {
    utils::ItpMsg msg;
    while (1) {
      if (recer_sender.ROLE(Recer).ROLE(ProxyRecer).ReceQueryMsg(msg) == false) {
        ERROR_LOG(" invalid msg, session is [%s], msg_name is [%s]", msg.session_name.c_str(), msg.msg_name.c_str());
        continue;
      }

      if (session_func_map.find(msg.session_name) != session_func_map.end()) {
        session_func_map[msg.session_name](msg);
        std::this_thread::sleep_for(1s);
      } else {
        ERROR_LOG("can not find[%s] in session_func_map", msg.session_name.c_str());
      }
    }
  };
  INFO_LOG("queryRecRun prepare ok");
  std::thread(query_rec_run).detach();

  auto itp_rec_run = [&]() {
    utils::ItpMsg msg;
    while (1) {
      if (recer_sender.ROLE(Recer).ROLE(ItpRecer).ReceMsg(msg) == false) {
        ERROR_LOG(" invalid msg, session is [%s], msg_name is [%s]", msg.session_name.c_str(), msg.msg_name.c_str());
        GlobalSem::GetInstance().PostSemBySemName(GlobalSem::kApiRecv);
        continue;
      }

      if (session_func_map.find(msg.session_name) != session_func_map.end()) {
        session_func_map[msg.session_name](msg);
      } else {
        ERROR_LOG("can not find[%s] in session_func_map", msg.session_name.c_str());
      }

      GlobalSem::GetInstance().PostSemBySemName(GlobalSem::kApiRecv);
    }
  };
  INFO_LOG("itpRecRun prepare ok");
  std::thread(itp_rec_run).detach();

  recer_sender.Run();

  return true;
}
