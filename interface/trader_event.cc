/*
 * traderEvent.cpp
 *
 *  Created on: 2020年9月20日
 *      Author: Administrator
 */

#include "trader/interface/trader_event.h"
#include <chrono>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/global_sem.h"
#include "trader/infra/recer_sender.h"

TraderEvent::TraderEvent() { RegSessionFunc(); }

TraderEvent::~TraderEvent() {}

void TraderEvent::RegSessionFunc() {
  session_func_map_.clear();
  session_func_map_["market_trader"] = [this](utils::ItpMsg msg) { ROLE(MarketEvent).Handle(msg); };
  session_func_map_["strategy_trader"] = [this](utils::ItpMsg msg) { ROLE(StrategyEvent).Handle(msg); };
  session_func_map_["ctp_trader"] = [this](utils::ItpMsg msg) { ROLE(CtpEvent).Handle(msg); };
  session_func_map_["otp_trader"] = [this](utils::ItpMsg msg) { ROLE(OtpEvent).Handle(msg); };
  session_func_map_["xtp_trader"] = [this](utils::ItpMsg msg) { ROLE(XtpEvent).Handle(msg); };
  session_func_map_["ftp_trader"] = [this](utils::ItpMsg msg) { ROLE(FtpEvent).Handle(msg); };
  session_func_map_["ctpview_trader"] = [this](utils::ItpMsg msg) { ROLE(CtpviewEvent).Handle(msg); };
  session_func_map_["btp_trader"] = [this](utils::ItpMsg msg) { ROLE(BtpEvent).Handle(msg); };
  session_func_map_["gtp_trader"] = [this](utils::ItpMsg msg) { ROLE(GtpEvent).Handle(msg); };
  session_func_map_["mtp_trader"] = [this](utils::ItpMsg msg) { ROLE(MtpEvent).Handle(msg); };
  session_func_map_["ytp_trader"] = [this](utils::ItpMsg msg) { ROLE(YtpEvent).Handle(msg); };
  session_func_map_["trader_trader"] = [this](utils::ItpMsg msg) { ROLE(SelfEvent).Handle(msg); };
}

bool TraderEvent::Run() {
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto api_type = json_cfg.GetConfig("common", "ApiType");
  running_ = true;

  order_rec_thread_ = std::thread(&TraderEvent::OrderRecTask, this);
  INFO_LOG("order rec thread start");

  query_rec_thread_ = std::thread(&TraderEvent::QueryRecTask, this);
  INFO_LOG("query rec thread start");

  itp_rec_thread_ = std::thread(&TraderEvent::ItpRecTask, this);
  INFO_LOG("itp rec thread start");

  if (api_type == "ftp") {
    delay_ms_ = 1;
  }

  return true;
}

bool TraderEvent::Stop() {
  running_ = false;
  if (order_rec_thread_.joinable()) {
    order_rec_thread_.join();
    INFO_LOG("order rec thread exit");
  }
  if (query_rec_thread_.joinable()) {
    query_rec_thread_.join();
    INFO_LOG("query rec thread exit");
  }
  if (itp_rec_thread_.joinable()) {
    itp_rec_thread_.join();
    INFO_LOG("itp rec thread exit");
  }
  return true;
}

void TraderEvent::OrderRecTask() {
  utils::ItpMsg msg;
  auto &recer_sender = RecerSender::GetInstance();
  while (running_) {
    if (!recer_sender.ROLE(Recer).ROLE(DirectRecer).ReceMsg(msg)) {
      continue;
    }

    if (session_func_map_.find(msg.session_name) != session_func_map_.end()) {
      session_func_map_[msg.session_name](msg);
    } else {
      ERROR_LOG("can not find[%s] in session func map", msg.session_name.c_str());
    }
  }
}

void TraderEvent::QueryRecTask() {
  utils::ItpMsg msg;
  auto &recer_sender = RecerSender::GetInstance();
  while (running_) {
    if (!recer_sender.ROLE(Recer).ROLE(ProxyRecer).ReceMsg(msg)) {
      continue;
    }

    if (session_func_map_.find(msg.session_name) != session_func_map_.end()) {
      session_func_map_[msg.session_name](msg);
      std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms_));
    } else {
      ERROR_LOG("can not find[%s] in session func map", msg.session_name.c_str());
    }
  }
}

void TraderEvent::ItpRecTask() {
  utils::ItpMsg msg;
  auto &recer_sender = RecerSender::GetInstance();
  while (running_) {
    if (!recer_sender.ROLE(Recer).ROLE(InnerRecer).ReceMsg(msg)) {
      continue;
    }

    if (session_func_map_.find(msg.session_name) != session_func_map_.end()) {
      session_func_map_[msg.session_name](msg);
    } else {
      ERROR_LOG("can not find[%s] in session func map", msg.session_name.c_str());
    }

    GlobalSem::GetInstance().PostSemBySemName(SemName::kApiRecv);
  }
}
