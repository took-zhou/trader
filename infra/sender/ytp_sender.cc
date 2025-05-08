#include "trader/infra/sender/ytp_sender.h"
#include <cstring>
#include <string>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/global_sem.h"
#include "common/self/utils.h"
#include "trader/infra/recer/ytp_recer.h"

std::map<int, YtpTraderInfo> YtpSender::ytp_trader_info_map;
ytp::api::TraderApi *YtpSender::trader_api;
YtpTraderSpi *YtpSender::trader_spi;

YtpSender::YtpSender() {}

YtpSender::~YtpSender(void) {}

bool YtpSender::ReqUserLogin() {
  INFO_LOG("login time, is going to login.");
  bool ret = true;
  if (!Init()) {
    Release();
    ret = false;
  } else {
    auto &json_cfg = utils::JsonConfig::GetInstance();
    auto users = json_cfg.GetConfig("trader", "User");
    for (auto &user : users) {
      const auto user_id = json_cfg.GetDeepConfig("users", user, "UserID").get<std::string>();

      YtpLoginLogoutStruct login;
      strcpy(login.user_id, user_id.c_str());
      uint64_t session = trader_api->Login(login);
      auto &global_sem = GlobalSem::GetInstance();
      global_sem.WaitSemBySemName(SemName::kLoginLogout);
      INFO_LOG("%s login ok", user_id.c_str());

      YtpTraderInfo trader_info;
      trader_info.user_id = user_id;
      trader_info.user_name = user;
      ytp_trader_info_map[session] = trader_info;
    }
  }

  return ret;
}

bool YtpSender::ReqUserLogout() {
  INFO_LOG("logout time, is going to logout.");

  auto &json_cfg = utils::JsonConfig::GetInstance();
  for (auto &item : ytp_trader_info_map) {
    const auto user_id = json_cfg.GetDeepConfig("users", item.second.user_name, "UserID").get<std::string>();

    YtpLoginLogoutStruct logout;
    strcpy(logout.user_id, user_id.c_str());

    if (trader_api != nullptr) {
      trader_api->Logout(logout);
      auto &global_sem = GlobalSem::GetInstance();
      global_sem.WaitSemBySemName(SemName::kLoginLogout);
      INFO_LOG("%s logout ok", user_id.c_str());
    }
  }

  Release();
  return true;
}

bool YtpSender::InsertOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = ytp_trader_info_map.find(content.session_id);
  if (pos != ytp_trader_info_map.end()) {
    YtpOrderInfoStruct orderinfo;
    orderinfo.order_ref = stoi(content.order_ref);
    orderinfo.price = content.limit_price;
    orderinfo.volume = content.once_volume;
    orderinfo.side = content.direction;
    orderinfo.comboffset = content.comboffset;
    strcpy(orderinfo.exchange_id, content.exchange_id.c_str());
    strcpy(orderinfo.instrument_id, content.instrument_id.c_str());
    strcpy(orderinfo.user_id, content.user_id.c_str());
    strcpy(orderinfo.index, content.index.c_str());
    trader_api->ReqOrderInsert(&orderinfo);
  } else {
    ret = false;
    ERROR_LOG("can not find session id: %ld", content.session_id);
  }
  return ret;
}

bool YtpSender::CancelOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = ytp_trader_info_map.find(content.session_id);
  if (pos != ytp_trader_info_map.end()) {
    YtpOrderInfoStruct orderinfo;
    orderinfo.order_ref = stoi(content.order_ref);
    orderinfo.price = content.limit_price;
    orderinfo.volume = content.once_volume - content.success_volume - content.fail_volume;
    strcpy(orderinfo.exchange_id, content.exchange_id.c_str());
    strcpy(orderinfo.instrument_id, content.instrument_id.c_str());
    strcpy(orderinfo.user_id, content.user_id.c_str());
    trader_api->ReqOrderAction(&orderinfo);
  }
  return ret;
}

bool YtpSender::Init(void) {
  bool out = true;

  if (!is_init_) {
    INFO_LOG("begin ytp trader api init");
    auto &json_cfg = utils::JsonConfig::GetInstance();
    trader_api = ytp::api::TraderApi::CreateTraderApi(json_cfg.GetFileName().c_str());
    if (trader_api == nullptr) {
      out = false;
      INFO_LOG("trader api init fail.");
    } else {
      out = true;
      trader_spi = new YtpTraderSpi();
      trader_api->RegisterSpi(trader_spi);
      INFO_LOG("trader api init ok.");
    }
    is_init_ = true;
  } else {
    ytp_trader_info_map.erase(ytp_trader_info_map.begin(), ytp_trader_info_map.end());
  }

  return out;
}

bool YtpSender::Release() {
  INFO_LOG("is going to release trader api.");

  if (trader_api != nullptr) {
    trader_api->Release();
    trader_api = nullptr;
  }

  // 释放UserSpi实例
  if (trader_spi != nullptr) {
    delete trader_spi;
    trader_spi = NULL;
  }

  ytp_trader_info_map.erase(ytp_trader_info_map.begin(), ytp_trader_info_map.end());
  is_init_ = false;

  return true;
}

bool YtpSender::ReqAvailableFunds() {
  if (trader_api != nullptr) {
    for (auto &item : ytp_trader_info_map) {
      YtpAccountField request_msg{{0}};
      strcpy(request_msg.user_id, item.second.user_id.c_str());
      int result = trader_api->QryTradingAccount(&request_msg, 0);
      if (result != 0) {
        INFO_LOG("qry trading account send result is [%d]", result);
      }
    }
  }

  return true;
}

bool YtpSender::ReqInstrumentInfo(const utils::InstrumentID &ins_exch) {
  INFO_LOG("req instrument info not support.");
  return true;
}

bool YtpSender::ReqTransactionCost(const utils::InstrumentID &ins_exch) {
  for (auto &item : ytp_trader_info_map) {
    YtpTransactionCostField field;
    strcpy(field.user_id, item.second.user_id.c_str());
    strcpy(field.exchange_id, ins_exch.exch.c_str());
    strcpy(field.instrument_id, ins_exch.ins.c_str());
    int result = trader_api->QryTransactionCost(&field, 0);
    INFO_LOG("req transaction cost send result is [%d]", result);
    break;
  }

  return true;
}

bool YtpSender::LossConnection() { return trader_spi->GetFrontDisconnected(); }
