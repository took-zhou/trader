#include "trader/infra/sender/gtp_sender.h"
#include <cstring>
#include <string>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/utils.h"
#include "trader/infra/recer/gtp_recer.h"

std::map<int, GtpTraderInfo> GtpSender::gtp_trader_info_map;
gtp::api::TraderApi *GtpSender::trader_api;
GtpTraderSpi *GtpSender::trader_spi;

GtpSender::GtpSender() { ; }

bool GtpSender::ReqUserLogin() {
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

      GtpLoginLogoutStruct login;
      strcpy(login.user_id, user_id.c_str());
      uint64_t session = trader_api->Login(login);
      INFO_LOG("%s login ok", user_id.c_str());

      GtpTraderInfo trader_info;
      trader_info.user_id = user_id;
      trader_info.user_name = user;
      gtp_trader_info_map[session] = trader_info;
    }
  }

  return ret;
}

bool GtpSender::ReqUserLogout() {
  INFO_LOG("logout time, is going to logout.");

  auto &json_cfg = utils::JsonConfig::GetInstance();
  for (auto &item : gtp_trader_info_map) {
    const auto user_id = json_cfg.GetDeepConfig("users", item.second.user_name, "UserID").get<std::string>();

    GtpLoginLogoutStruct logout;
    strcpy(logout.user_id, user_id.c_str());

    if (trader_api != nullptr) {
      trader_api->Logout(logout);
      INFO_LOG("%s logout ok", user_id.c_str());
    }
  }

  Release();
  return true;
}

bool GtpSender::InsertOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = gtp_trader_info_map.find(content.session_id);
  if (pos != gtp_trader_info_map.end()) {
    GtpOrderInfoStruct orderinfo;
    orderinfo.order_ref = stoi(content.order_ref);
    orderinfo.price = content.limit_price;
    orderinfo.volume = content.total_volume;
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

bool GtpSender::CancelOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = gtp_trader_info_map.find(content.session_id);
  if (pos != gtp_trader_info_map.end()) {
    GtpOrderInfoStruct orderinfo;
    orderinfo.order_ref = stoi(content.order_ref);
    orderinfo.price = content.limit_price;
    orderinfo.volume = content.total_volume - content.success_volume - content.fail_volume;
    strcpy(orderinfo.exchange_id, content.exchange_id.c_str());
    strcpy(orderinfo.instrument_id, content.instrument_id.c_str());
    strcpy(orderinfo.user_id, content.user_id.c_str());
    trader_api->ReqOrderAction(&orderinfo);
  }
  return ret;
}

bool GtpSender::Init(void) {
  bool out = true;

  if (!is_init_) {
    INFO_LOG("begin GtpTraderApi init");
    auto &json_cfg = utils::JsonConfig::GetInstance();
    trader_api = gtp::api::TraderApi::CreateTraderApi(json_cfg.GetFileName().c_str());
    if (trader_api == nullptr) {
      out = false;
      INFO_LOG("traderApi init fail.");
    } else {
      out = true;
      trader_spi = new GtpTraderSpi();
      trader_api->RegisterSpi(trader_spi);
      INFO_LOG("traderApi init ok.");
    }
    is_init_ = true;
  }

  return out;
}

bool GtpSender::Release() {
  INFO_LOG("Is going to release traderApi.");

  if (trader_api != nullptr) {
    trader_api->Release();
    trader_api = nullptr;
  }

  // 释放UserSpi实例
  if (trader_spi != nullptr) {
    delete trader_spi;
    trader_spi = NULL;
  }

  gtp_trader_info_map.erase(gtp_trader_info_map.begin(), gtp_trader_info_map.end());
  is_init_ = false;

  return true;
}

bool GtpSender::ReqAvailableFunds() {
  if (trader_api != nullptr) {
    for (auto &item : gtp_trader_info_map) {
      GtpAccountField request_msg{{0}};
      strcpy(request_msg.user_id, item.second.user_id.c_str());
      int result = trader_api->QryTradingAccount(&request_msg, 0);
      if (result != 0) {
        INFO_LOG("QryTradingAccount send result is [%d]", result);
      }
    }
  }

  return true;
}

bool GtpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch) {
  INFO_LOG("ReqInstrumentInfo not support.");
  return true;
}

bool GtpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch) {
  for (auto &item : gtp_trader_info_map) {
    GtpTransactionCostField field;
    strcpy(field.user_id, item.second.user_id.c_str());
    strcpy(field.exchange_id, ins_exch.exch.c_str());
    strcpy(field.instrument_id, ins_exch.ins.c_str());
    int result = trader_api->QryTransactionCost(&field, 0);
    INFO_LOG("ReqTransactionCost send result is [%d]", result);
    break;
  }

  return true;
}

bool GtpSender::LossConnection() { return false; }
