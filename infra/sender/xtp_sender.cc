#include "trader/infra/sender/xtp_sender.h"
#include <string>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/infra/recer/xtp_recer.h"

std::map<int, XtpTraderInfo> XtpSender::xtp_trader_info_map;
XTP::API::TraderApi *XtpSender::trader_api;
XtpTraderSpi *XtpSender::trader_spi;

XtpSender::XtpSender() { ; }

bool XtpSender::ReqUserLogin() {
  INFO_LOG("login time, is going to login.");
  bool ret = true;
  Init();
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto users = json_cfg.GetConfig("trader", "User");
  for (auto &user : users) {
    const std::string frontaddr = json_cfg.GetDeepConfig("users", user, "FrontAddr").get<std::string>();
    const std::string user_id = json_cfg.GetDeepConfig("users", user, "UserID").get<std::string>();
    const std::string password = json_cfg.GetDeepConfig("users", user, "Password").get<std::string>();

    auto protocol_ip_port = utils::SplitString(frontaddr, ":");
    std::string ip_str = utils::SplitString(protocol_ip_port[1], "//")[1];
    int port = stoi(protocol_ip_port[2]);
    XTP_PROTOCOL_TYPE protoc = XTP_PROTOCOL_TCP;
    if (protocol_ip_port[0] == "tcp") {
      protoc = XTP_PROTOCOL_TCP;
    } else if (protocol_ip_port[0] == "udp") {
      protoc = XTP_PROTOCOL_UDP;
    }

    uint64_t session = trader_api->Login(ip_str.c_str(), port, user_id.c_str(), password.c_str(), protoc);
    if (session == 0) {
      XTPRI *error_info = trader_api->GetApiLastError();
      ERROR_LOG("Login to server error: %d : %s", error_info->error_id, error_info->error_msg);
      Release();
      ret = false;
    } else {
      auto &global_sem = GlobalSem::GetInstance();
      if (global_sem.WaitSemBySemName(GlobalSem::kLoginLogout, 1) != 0) {
        INFO_LOG("%s login ok", user_id.c_str());
        trader_spi->OnRspUserLogin();
      }
      XtpTraderInfo trader_info;
      trader_info.user_id = user_id;
      trader_info.user_name = user;
      xtp_trader_info_map[session] = trader_info;
    }
  }

  return ret;
}

bool XtpSender::ReqUserLogout() {
  INFO_LOG("logout time, is going to logout.");

  auto &json_cfg = utils::JsonConfig::GetInstance();
  for (auto &item : xtp_trader_info_map) {
    const std::string user_id = json_cfg.GetDeepConfig("users", item.second.user_name, "UserID").get<std::string>();
    if (trader_api != nullptr) {
      int result = trader_api->Logout(item.first);
      INFO_LOG("ReqUserLogout send result is [%d]", result);

      auto &global_sem = GlobalSem::GetInstance();
      if (global_sem.WaitSemBySemName(GlobalSem::kLoginLogout, 3) != 0) {
        trader_spi->OnRspUserLogout();
      }
    }
  }

  Release();
  return true;
}

bool XtpSender::InsertOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = xtp_trader_info_map.find(content.session_id);
  if (pos != xtp_trader_info_map.end()) {
    XTPOrderInsertInfo orderinfo;
    orderinfo.order_client_id = stoi(content.order_ref);
    strcpy(orderinfo.ticker, content.instrument_id.c_str());
    if (content.exchange_id == "SHSE") {
      orderinfo.market = XTP_MKT_SH_A;
    } else if (content.exchange_id == "SZSE") {
      orderinfo.market = XTP_MKT_SZ_A;
    }
    orderinfo.price = content.limit_price;
    orderinfo.quantity = content.total_volume;

    if (content.comboffset == strategy_trader::OPEN) {
      orderinfo.side = XTP_SIDE_BUY;
    } else if (content.comboffset == strategy_trader::CLOSE) {
      orderinfo.side = XTP_SIDE_SELL;
    } else if (content.comboffset == strategy_trader::CLOSE_YESTERDAY) {
      orderinfo.side = XTP_SIDE_SELL;
    } else if (content.comboffset == strategy_trader::CLOSE_TODAY) {
      orderinfo.side = XTP_SIDE_SELL;
    }

    orderinfo.price_type = (XTP_PRICE_TYPE)1;
    orderinfo.business_type = (XTP_BUSINESS_TYPE)0;
    orderinfo.position_effect = (XTP_POSITION_EFFECT_TYPE)0;
    content.xtp_order_id = trader_api->InsertOrder(&orderinfo, pos->first);
  } else {
    ret = false;
    ERROR_LOG("can not find session id: %ld", content.session_id);
  }
  return ret;
}

bool XtpSender::CancelOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = xtp_trader_info_map.find(content.session_id);
  if (pos != xtp_trader_info_map.end()) {
    trader_api->CancelOrder(content.xtp_order_id, pos->first);
  }
  return ret;
}

bool XtpSender::Init(void) {
  bool out = true;

  if (is_init_ == false) {
    auto &json_cfg = utils::JsonConfig::GetInstance();
    uint8_t client_id = json_cfg.GetConfig("common", "ClientId").get<std::uint8_t>();
    auto users = json_cfg.GetConfig("trader", "User");
    for (auto &user : users) {
      INFO_LOG("begin CtpTraderApi init");
      std::string con_path = json_cfg.GetConfig("trader", "ConPath").get<std::string>() + "/" + (std::string)user + "/";
      utils::CreatFolder(con_path);

      trader_api = XTP::API::TraderApi::CreateTraderApi(client_id, con_path.c_str(), XTP_LOG_LEVEL_DEBUG);
      INFO_LOG("xtp version: %s.", trader_api->GetApiVersion());

      const std::string auth_code = json_cfg.GetDeepConfig("users", (std::string)user, "AuthCode").get<std::string>();
      trader_api->SetSoftwareKey(auth_code.c_str());

      trader_spi = new XtpTraderSpi();
      trader_api->RegisterSpi(trader_spi);

      INFO_LOG("traderApi init ok.");
      break;
    }
    is_init_ = true;
  }

  return out;
}

bool XtpSender::Release() {
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

  xtp_trader_info_map.erase(xtp_trader_info_map.begin(), xtp_trader_info_map.end());
  is_init_ = false;

  return true;
}

bool XtpSender::ReqAvailableFunds() {
  bool ret = true;
  for (auto &item : xtp_trader_info_map) {
    if (trader_api != nullptr) {
      ret = trader_api->QueryAsset(item.first, 0);
    }
  }
  return ret;
}

bool XtpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch) {
  INFO_LOG("ReqInstrumentInfo not support.");
  return true;
}
bool XtpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch) {
  INFO_LOG("ReqTransactionCost not support.");
  return true;
}

bool XtpSender::LossConnection() {
  bool ret = false;
  if (trader_spi != nullptr && trader_spi->front_disconnected == true) {
    ret = true;
  }

  return ret;
}