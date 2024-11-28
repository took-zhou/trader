#include "trader/infra/sender/mtp_sender.h"
#include <cstring>
#include <string>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/utils.h"
#include "trader/infra/recer/mtp_recer.h"

std::map<int, MtpTraderInfo> MtpSender::mtp_trader_info_map;
mtp::api::TraderApi *MtpSender::trader_api;
MtpTraderSpi *MtpSender::trader_spi;

MtpSender::MtpSender() {}

MtpSender::~MtpSender(void) {}

bool MtpSender::ReqUserLogin() {
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

      MtpLoginLogoutStruct login;
      strcpy(login.user_id, user_id.c_str());
      uint64_t session = trader_api->Login(login);
      INFO_LOG("%s login ok", user_id.c_str());

      MtpTraderInfo trader_info;
      trader_info.user_id = user_id;
      trader_info.user_name = user;
      mtp_trader_info_map[session] = trader_info;
    }
  }

  return ret;
}

bool MtpSender::ReqUserLogout() {
  INFO_LOG("logout time, is going to logout.");

  auto &json_cfg = utils::JsonConfig::GetInstance();
  for (auto &item : mtp_trader_info_map) {
    const auto user_id = json_cfg.GetDeepConfig("users", item.second.user_name, "UserID").get<std::string>();

    MtpLoginLogoutStruct logout;
    strcpy(logout.user_id, user_id.c_str());

    if (trader_api != nullptr) {
      trader_api->Logout(logout);
      INFO_LOG("%s logout ok", user_id.c_str());
    }
  }

  Release();
  return true;
}

bool MtpSender::InsertOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = mtp_trader_info_map.find(content.session_id);
  if (pos != mtp_trader_info_map.end()) {
    MtpOrderInfoStruct orderinfo;
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

bool MtpSender::CancelOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = mtp_trader_info_map.find(content.session_id);
  if (pos != mtp_trader_info_map.end()) {
    MtpOrderInfoStruct orderinfo;
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

bool MtpSender::Init(void) {
  bool out = true;

  if (!is_init_) {
    INFO_LOG("begin MtpTraderApi init");
    auto &json_cfg = utils::JsonConfig::GetInstance();
    trader_api = mtp::api::TraderApi::CreateTraderApi(json_cfg.GetFileName().c_str());
    if (trader_api == nullptr) {
      out = false;
      INFO_LOG("traderApi init fail.");
    } else {
      out = true;
      trader_spi = new MtpTraderSpi();
      trader_api->RegisterSpi(trader_spi);
      INFO_LOG("traderApi init ok.");
    }
    is_init_ = true;
  } else {
    mtp_trader_info_map.erase(mtp_trader_info_map.begin(), mtp_trader_info_map.end());
  }

  return out;
}

bool MtpSender::Release() {
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

  mtp_trader_info_map.erase(mtp_trader_info_map.begin(), mtp_trader_info_map.end());
  is_init_ = false;

  return true;
}

bool MtpSender::ReqAvailableFunds() {
  if (trader_api != nullptr) {
    for (auto &item : mtp_trader_info_map) {
      MtpAccountField request_msg{{0}};
      strcpy(request_msg.user_id, item.second.user_id.c_str());
      int result = trader_api->QryTradingAccount(&request_msg, 0);
      if (result != 0) {
        INFO_LOG("QryTradingAccount send result is [%d]", result);
      }
    }
  }

  return true;
}

bool MtpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch) {
  INFO_LOG("ReqInstrumentInfo not support.");
  return true;
}

bool MtpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch) {
  for (auto &item : mtp_trader_info_map) {
    MtpTransactionCostField field;
    strcpy(field.user_id, item.second.user_id.c_str());
    strcpy(field.exchange_id, ins_exch.exch.c_str());
    strcpy(field.instrument_id, ins_exch.ins.c_str());
    int result = trader_api->QryTransactionCost(&field, 0);
    INFO_LOG("ReqTransactionCost send result is [%d]", result);
    break;
  }

  return true;
}

bool MtpSender::LossConnection() { return trader_spi->GetFrontDisconnected(); }
