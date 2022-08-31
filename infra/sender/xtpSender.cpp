#include "trader/infra/sender/xtpSender.h"
#include <string>
#include <thread>
#include "common/extern/log/log.h"
#include "common/self/fileUtil.h"
#include "common/self/semaphorePart.h"
#include "common/self/utils.h"
#include "trader/infra/recer/xtpRecer.h"

std::map<std::string, XtpTraderInfo> XtpSender::XtpTraderInfoMap;

XtpSender::XtpSender() { ; }

bool XtpSender::ReqUserLogin() {
  INFO_LOG("login time, is going to login.");
  bool ret = true;
  init();
  auto &jsonCfg = utils::JsonConfig::getInstance();
  for (auto &item : XtpTraderInfoMap) {
    const std::string frontaddr = jsonCfg.getDeepConfig("users", item.first, "FrontAddr").get<std::string>();
    const std::string userID = jsonCfg.getDeepConfig("users", item.first, "UserID").get<std::string>();
    const std::string password = jsonCfg.getDeepConfig("users", item.first, "Password").get<std::string>();

    auto protocol_ip_port = utils::splitString(frontaddr, ":");
    std::string ip = utils::splitString(protocol_ip_port[1], "//")[1];
    int port = utils::stringToInt(protocol_ip_port[2]);
    XTP_PROTOCOL_TYPE protoc = XTP_PROTOCOL_TCP;
    if (protocol_ip_port[0] == "tcp") {
      protoc = XTP_PROTOCOL_TCP;
    } else if (protocol_ip_port[0] == "udp") {
      protoc = XTP_PROTOCOL_UDP;
    }

    item.second.sessionId = item.second.traderApi->Login(ip.c_str(), port, userID.c_str(), password.c_str(), protoc);
    if (item.second.sessionId == 0) {
      XTPRI *error_info = item.second.traderApi->GetApiLastError();
      ERROR_LOG("Login to server error: %d : %s", error_info->error_id, error_info->error_msg);
      release();
      ret = false;
    } else {
      auto &globalSem = GlobalSem::getInstance();
      if (globalSem.waitSemBySemName(GlobalSem::loginLogout, 3) != 0) {
        item.second.traderSpi->OnRspUserLogin();
      }
    }
  }

  return ret;
}

bool XtpSender::ReqUserLogout() {
  INFO_LOG("logout time, is going to logout.");

  for (auto &item : XtpTraderInfoMap) {
    int result = item.second.traderApi->Logout(item.second.sessionId);
    INFO_LOG("ReqUserLogout send result is [%d]", result);

    auto &globalSem = GlobalSem::getInstance();
    if (globalSem.waitSemBySemName(GlobalSem::loginLogout, 3) != 0) {
      item.second.traderSpi->OnRspUserLogin();
    }
  }

  release();
}

bool XtpSender::InsertOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = XtpTraderInfoMap.find(content.userId);
  if (pos != XtpTraderInfoMap.end()) {
    XTPOrderInsertInfo orderinfo;
    orderinfo.order_client_id = pos->second.sessionId;
    strcpy(orderinfo.ticker, content.instrumentID.c_str());
    if (content.exchangeId == "SHSE") {
      orderinfo.market = XTP_MKT_SH_A;
    } else if (content.exchangeId == "SZSE") {
      orderinfo.market = XTP_MKT_SZ_A;
    }

    orderinfo.price = content.limit_price;
    orderinfo.quantity = content.total_volume;
    orderinfo.side = content.comboffset;
    orderinfo.price_type = (XTP_PRICE_TYPE)1;
    orderinfo.business_type = (XTP_BUSINESS_TYPE)0;
    orderinfo.position_effect = (XTP_POSITION_EFFECT_TYPE)0;

    int64_t xtp_id = pos->second.traderApi->InsertOrder(&orderinfo, pos->second.sessionId);
    content.orderRef = utils::unsignedlongintToString(xtp_id);
  }
  return ret;
}

bool XtpSender::CancelOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = XtpTraderInfoMap.find(content.userId);
  if (pos != XtpTraderInfoMap.end()) {
    pos->second.traderApi->CancelOrder(utils::stringToInt(content.orderRef), pos->second.sessionId);
  }
  return ret;
}

bool XtpSender::init(void) {
  bool out = true;
  if (isInit == false) {
    auto &jsonCfg = utils::JsonConfig::getInstance();
    uint8_t client_id = jsonCfg.getConfig("common", "ClientId").get<std::uint8_t>();
    auto users = jsonCfg.getConfig("market", "User");
    for (auto &user : users) {
      std::string conPath = jsonCfg.getConfig("trader", "ConPath").get<std::string>() + "/" + (std::string)user + "/";
      utils::creatFolder(conPath);

      XtpTraderInfo traderInfo;
      traderInfo.traderApi = XTP::API::TraderApi::CreateTraderApi(client_id, conPath.c_str(), XTP_LOG_LEVEL_DEBUG);
      INFO_LOG("xtp version: %s.", traderInfo.traderApi->GetApiVersion());

      const std::string authCode = jsonCfg.getDeepConfig("users", (std::string)user, "AuthCode").get<std::string>();
      traderInfo.traderApi->SetSoftwareKey(authCode.c_str());

      traderInfo.traderSpi = new XtpTraderSpi();
      traderInfo.traderApi->RegisterSpi(traderInfo.traderSpi);

      XtpTraderInfoMap[(std::string)user] = traderInfo;

      INFO_LOG("traderApi init ok.");
      std::this_thread::sleep_for(1000ms);
    }
    isInit = true;
  }
  return out;
}

bool XtpSender::release() {
  INFO_LOG("Is going to release traderApi.");

  for (auto &item : XtpTraderInfoMap) {
    item.second.traderApi->Release();
    item.second.traderApi = nullptr;

    // 释放UserSpi实例
    if (item.second.traderSpi) {
      delete item.second.traderSpi;
      item.second.traderSpi = NULL;
    }
  }
}

bool XtpSender::ReqAvailableFunds(const int requestId) {}
bool XtpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int requestId) {}
bool XtpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int requestId) {}

bool XtpSender::LossConnection() {
  bool ret = false;
  for (auto &item : XtpTraderInfoMap) {
    if (item.second.traderSpi != nullptr && item.second.traderSpi->frontDisconnected == true) {
      ret = true;
    }
    break;
  }
  return ret;
}