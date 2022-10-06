#include "trader/infra/sender/btp_sender.h"
#include <string>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/infra/recer/btp_recer.h"

std::map<int, BtpTraderInfo> BtpSender::btp_trader_info_map;
btp::api::TraderApi *BtpSender::trader_api;
BtpTraderSpi *BtpSender::trader_spi;

BtpSender::BtpSender() { ; }

bool BtpSender::ReqUserLogin() {
  INFO_LOG("login time, is going to login.");
  bool ret = true;
  Init();
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto users = json_cfg.GetConfig("trader", "User");
  for (auto &user : users) {
    const std::string user_id = json_cfg.GetDeepConfig("users", user, "UserID").get<std::string>();

    int result = trader_api->Login();
    INFO_LOG("ReqUserLogin send result is [%d]", result);
    int session = 1234567890;
    BtpTraderInfo trader_info;
    trader_info.user_id = user_id;
    trader_info.user_name = user;
    btp_trader_info_map[session] = trader_info;
  }

  return ret;
}

bool BtpSender::ReqUserLogout() {
  INFO_LOG("logout time, is going to logout.");

  auto &json_cfg = utils::JsonConfig::GetInstance();
  for (auto &item : btp_trader_info_map) {
    const std::string user_id = json_cfg.GetDeepConfig("users", item.second.user_name, "UserID").get<std::string>();
    if (trader_api != nullptr) {
      int result = trader_api->Logout();
      INFO_LOG("ReqUserLogout send result is [%d]", result);
    }
  }

  Release();
  return true;
}

bool BtpSender::InsertOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = btp_trader_info_map.find(content.session_id);
  if (pos != btp_trader_info_map.end()) {
    content.user_id = pos->second.user_id;

    BtpOrderInfoStruct orderinfo;
    orderinfo.order_ref = stoi(content.order_ref);
    orderinfo.price = content.limit_price;
    orderinfo.volume = content.total_volume;
    orderinfo.side = content.direction;

    trader_api->ReqOrderInsert(&orderinfo);
  } else {
    ret = false;
    ERROR_LOG("can not find session id: %ld", content.session_id);
  }
  return ret;
}

bool BtpSender::CancelOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = btp_trader_info_map.find(content.session_id);
  if (pos != btp_trader_info_map.end()) {
    BtpOrderInfoStruct orderinfo;
    orderinfo.order_ref = stoi(content.order_ref);
    orderinfo.price = content.limit_price;
    orderinfo.volume = content.total_volume;
    trader_api->ReqOrderAction(&orderinfo);
  }
  return ret;
}

bool BtpSender::Init(void) {
  bool out = true;

  if (is_init_ == false) {
    auto &json_cfg = utils::JsonConfig::GetInstance();
    auto users = json_cfg.GetConfig("trader", "User");
    for (auto &user : users) {
      INFO_LOG("begin BtpTraderApi init");
      trader_api = btp::api::TraderApi::CreateTraderApi();

      trader_spi = new BtpTraderSpi();
      trader_api->RegisterSpi(trader_spi);
      INFO_LOG("traderApi init ok.");
      break;
    }
    is_init_ = true;
  }

  return out;
}

bool BtpSender::Release() {
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
  is_init_ = false;

  return true;
}

bool BtpSender::ReqAvailableFunds(const int request_id) {
  int result = trader_api->QryTradingAccount(request_id);
  INFO_LOG("ReqAvailableFunds send result is [%d]", result);
  return true;
}

bool BtpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int request_id) {
  INFO_LOG("ReqInstrumentInfo not support.");
  return true;
}

bool BtpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int request_id) {
  BtpTransactionCostField field;
  strcpy(field.exchange_id, ins_exch.exch.c_str());
  strcpy(field.instrument_id, ins_exch.ins.c_str());

  int result = trader_api->QryTransactionCost(&field, request_id);
  INFO_LOG("ReqTransactionCost send result is [%d]", result);
  return true;
}

bool BtpSender::LossConnection() { return false; }
