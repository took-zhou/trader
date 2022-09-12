#include "trader/infra/sender/xtp_sender.h"
#include <string>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/infra/recer/xtp_recer.h"

std::map<int, XtpTraderInfo> XtpSender::kXtpTraderInfoMap;
XTP::API::TraderApi *XtpSender::kTraderApi;
XtpTraderSpi *XtpSender::kTraderSpi;

XtpSender::XtpSender() { ; }

bool XtpSender::ReqUserLogin() {
  INFO_LOG("login time, is going to login.");
  bool ret = true;
  init();
  auto &jsonCfg = utils::JsonConfig::getInstance();
  auto users = jsonCfg.get_config("trader", "User");
  for (auto &user : users) {
    const std::string frontaddr = jsonCfg.get_deep_config("users", user, "FrontAddr").get<std::string>();
    const std::string userID = jsonCfg.get_deep_config("users", user, "UserID").get<std::string>();
    const std::string password = jsonCfg.get_deep_config("users", user, "Password").get<std::string>();

    auto protocol_ip_port = utils::SplitString(frontaddr, ":");
    std::string ip = utils::SplitString(protocol_ip_port[1], "//")[1];
    int port = stoi(protocol_ip_port[2]);
    XTP_PROTOCOL_TYPE protoc = XTP_PROTOCOL_TCP;
    if (protocol_ip_port[0] == "tcp") {
      protoc = XTP_PROTOCOL_TCP;
    } else if (protocol_ip_port[0] == "udp") {
      protoc = XTP_PROTOCOL_UDP;
    }

    uint64_t session = kTraderApi->Login(ip.c_str(), port, userID.c_str(), password.c_str(), protoc);
    if (session == 0) {
      XTPRI *error_info = kTraderApi->GetApiLastError();
      ERROR_LOG("Login to server error: %d : %s", error_info->error_id, error_info->error_msg);
      release();
      ret = false;
    } else {
      auto &globalSem = GlobalSem::getInstance();
      if (globalSem.WaitSemBySemName(GlobalSem::kLoginLogout, 1) != 0) {
        INFO_LOG("%s login ok", userID.c_str());
        kTraderSpi->OnRspUserLogin();
      }
      XtpTraderInfo traderInfo;
      traderInfo.user_id = userID;
      kXtpTraderInfoMap[session] = traderInfo;
    }
  }

  return ret;
}

bool XtpSender::ReqUserLogout() {
  INFO_LOG("logout time, is going to logout.");

  auto &jsonCfg = utils::JsonConfig::getInstance();
  for (auto &item : kXtpTraderInfoMap) {
    const std::string userID = jsonCfg.get_deep_config("users", item.second.user_id, "UserID").get<std::string>();
    int result = kTraderApi->Logout(item.first);
    INFO_LOG("ReqUserLogout send result is [%d]", result);

    auto &globalSem = GlobalSem::getInstance();
    if (globalSem.WaitSemBySemName(GlobalSem::kLoginLogout, 3) != 0) {
      kTraderSpi->OnRspUserLogout();
    }
  }

  release();
}

bool XtpSender::InsertOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = kXtpTraderInfoMap.find(content.sessionId);
  if (pos != kXtpTraderInfoMap.end()) {
    XTPOrderInsertInfo orderinfo;
    orderinfo.order_client_id = stoi(content.orderRef);
    strcpy(orderinfo.ticker, content.instrumentID.c_str());
    if (content.exchangeId == "SHSE") {
      orderinfo.market = XTP_MKT_SH_A;
    } else if (content.exchangeId == "SZSE") {
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
    content.xtpOrderId = kTraderApi->InsertOrder(&orderinfo, pos->first);
  }
  return ret;
}

bool XtpSender::CancelOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = kXtpTraderInfoMap.find(content.sessionId);
  if (pos != kXtpTraderInfoMap.end()) {
    kTraderApi->CancelOrder(content.xtpOrderId, pos->first);
  }
  return ret;
}

bool XtpSender::init(void) {
  bool out = true;

  if (is_init == false) {
    auto &jsonCfg = utils::JsonConfig::getInstance();
    uint8_t client_id = jsonCfg.get_config("common", "ClientId").get<std::uint8_t>();
    auto users = jsonCfg.get_config("trader", "User");
    for (auto &user : users) {
      INFO_LOG("begin CtpTraderApi init");
      std::string conPath = jsonCfg.get_config("trader", "ConPath").get<std::string>() + "/" + (std::string)user + "/";
      utils::CreatFolder(conPath);

      kTraderApi = XTP::API::TraderApi::CreateTraderApi(client_id, conPath.c_str(), XTP_LOG_LEVEL_DEBUG);
      INFO_LOG("xtp version: %s.", kTraderApi->GetApiVersion());

      const std::string authCode = jsonCfg.get_deep_config("users", (std::string)user, "AuthCode").get<std::string>();
      kTraderApi->SetSoftwareKey(authCode.c_str());

      kTraderSpi = new XtpTraderSpi();
      kTraderApi->RegisterSpi(kTraderSpi);

      INFO_LOG("traderApi init ok.");
      break;
    }
    is_init = true;
  }

  return out;
}

bool XtpSender::release() {
  INFO_LOG("Is going to release traderApi.");

  kTraderApi->Release();
  kTraderApi = nullptr;

  // 释放UserSpi实例
  if (kTraderSpi) {
    delete kTraderSpi;
    kTraderSpi = NULL;
  }
}

bool XtpSender::ReqAvailableFunds(const int requestId) {
  bool ret = true;
  for (auto &item : kXtpTraderInfoMap) {
    ret = kTraderApi->QueryAsset(item.first, requestId);
  }
  return ret;
}

bool XtpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int requestId) {
  INFO_LOG("ReqInstrumentInfo not support.");
  return true;
}
bool XtpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int requestId) {
  INFO_LOG("ReqTransactionCost not support.");
  return true;
}

bool XtpSender::LossConnection() {
  bool ret = false;
  for (auto &item : kXtpTraderInfoMap) {
    if (kTraderSpi != nullptr && kTraderSpi->front_disconnected == true) {
      ret = true;
    }
    break;
  }
  return ret;
}