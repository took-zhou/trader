/*
 * ctpSender.cpp
 *
 *  Created on: 2020.08.29
 *      Author: Administrator
 */

#include "trader/infra/sender/ctpSender.h"
#include <fstream>
#include <string>
#include <thread>
#include "common/extern/json/json.h"
#include "common/extern/log/log.h"
#include "common/self/fileUtil.h"
#include "common/self/semaphorePart.h"
#include "common/self/utils.h"
#include "trader/infra/recer/ctpRecer.h"

std::map<std::string, CtpTraderInfo> CtpSender::CtpTraderInfoMap;
CThostFtdcInputOrderField CtpSender::defaultOrderField;

CtpSender::CtpSender(void) {
  defaultOrderField.MinVolume = 1;
  defaultOrderField.ContingentCondition = THOST_FTDC_CC_Immediately;
  defaultOrderField.StopPrice = 0;
  defaultOrderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
  defaultOrderField.IsAutoSuspend = 0;
  defaultOrderField.UserForceClose = 0;
  defaultOrderField.IsSwapOrder = 0;
  defaultOrderField.CombHedgeFlag[0] = '1';

  std::string mac;
  if (!utils::get_local_mac(mac)) {
    ERROR_LOG("get_local_mac error!");
  }
  strcpy(defaultOrderField.MacAddress, mac.c_str());

  std::string Ip;
  std::string hostName;
  if (!utils::GetHostInfo(hostName, Ip)) {
    ERROR_LOG("GetHostInfo error!");
  }
  strcpy(defaultOrderField.IPAddress, Ip.c_str());
}

bool CtpSender::ReqUserLogin() {
  bool ret = true;
  if (init() == false) {
    release();
    ret = false;
  } else {
    authenticate();

    for (auto &item : CtpTraderInfoMap) {
      CThostFtdcReqUserLoginField reqUserLogin{0};
      auto &jsonCfg = utils::JsonConfig::getInstance();
      const std::string userID = jsonCfg.getDeepConfig("users", item.first, "UserID").get<std::string>();
      const std::string brokerID = jsonCfg.getDeepConfig("users", item.first, "BrokerID").get<std::string>();
      const std::string passWord = jsonCfg.getDeepConfig("users", item.first, "Password").get<std::string>();
      strcpy(reqUserLogin.BrokerID, brokerID.c_str());
      strcpy(reqUserLogin.UserID, userID.c_str());
      strcpy(reqUserLogin.Password, passWord.c_str());

      auto &globalSem = GlobalSem::getInstance();
      int result = item.second.traderApi->ReqUserLogin(&reqUserLogin, nRequestID++);
      INFO_LOG("ReqUserLogin send result is [%d]", result);
      globalSem.waitSemBySemName(GlobalSem::loginLogout);
      INFO_LOG("login ctp ok!");
    }
    confirm();
    return true;
  }
}
bool CtpSender::ReqUserLogout() {
  for (auto &item : CtpTraderInfoMap) {
    CThostFtdcUserLogoutField logOutField{0};
    auto &jsonCfg = utils::JsonConfig::getInstance();
    const std::string userID = jsonCfg.getDeepConfig("users", item.first, "UserID").get<std::string>();
    const std::string brokerID = jsonCfg.getDeepConfig("users", item.first, "BrokerID").get<std::string>();
    strcpy(logOutField.BrokerID, brokerID.c_str());
    strcpy(logOutField.UserID, userID.c_str());

    auto &globalSem = GlobalSem::getInstance();
    int result = item.second.traderApi->ReqUserLogout(&logOutField, nRequestID++);
    INFO_LOG("ReqUserLogout send result is [%d]", result);

    globalSem.waitSemBySemName(GlobalSem::loginLogout, 10);
  }
  release();
  INFO_LOG("ctp log out ok!");
}

bool CtpSender::InsertOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = CtpTraderInfoMap.find(content.userId);
  if (pos != CtpTraderInfoMap.end()) {
    CThostFtdcInputOrderField order;
    buildorder(content, order);

    auto &jsonCfg = utils::JsonConfig::getInstance();
    const std::string brokerId = jsonCfg.getDeepConfig("users", pos->first, "BrokerID").get<std::string>();
    const std::string investorId = jsonCfg.getDeepConfig("users", pos->first, "InvestorID").get<std::string>();
    const std::string userId = jsonCfg.getDeepConfig("users", pos->first, "UserID").get<std::string>();

    strcpy(order.BrokerID, brokerId.c_str());
    strcpy(order.InvestorID, investorId.c_str());
    strcpy(order.UserID, userId.c_str());

    int result = pos->second.traderApi->ReqOrderInsert(&order, nRequestID++);
    INFO_LOG("ReqOrderInsert send result is [%d], orderRef is[%s]", result, order.OrderRef);
  }

  return ret;
}

bool CtpSender::CancelOrder(utils::OrderContent &content) {
  bool ret = true;
  auto pos = CtpTraderInfoMap.find(content.userId);
  if (pos != CtpTraderInfoMap.end()) {
    CThostFtdcInputOrderActionField orderActionReq{0};
    strcpy(orderActionReq.OrderRef, content.orderRef.c_str());
    orderActionReq.FrontID = pos->second.frontId;
    orderActionReq.SessionID = pos->second.sessionId;
    strcpy(orderActionReq.InstrumentID, content.instrumentID.c_str());
    strcpy(orderActionReq.InvestorID, content.userId.c_str());
    strcpy(orderActionReq.UserID, content.userId.c_str());
    orderActionReq.ActionFlag = THOST_FTDC_AF_Delete;
    int result = pos->second.traderApi->ReqOrderAction(&orderActionReq, nRequestID++);
    INFO_LOG("ReqOrderAction send result is [%d]", result);
  }

  return ret;
}

bool CtpSender::ReqAvailableFunds(const int requestId) {}
bool CtpSender::ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int requestId) {}
bool CtpSender::ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int requestId) {}

bool CtpSender::init() {
  bool out = true;
  if (isInit == false) {
    INFO_LOG("begin CtpTraderApi init");

    auto &jsonCfg = utils::JsonConfig::getInstance();
    auto users = jsonCfg.getConfig("market", "User");
    for (auto &user : users) {
      std::string conPath = jsonCfg.getConfig("trader", "ConPath").get<std::string>() + "/" + (std::string)user + "/";
      utils::creatFolder(conPath);

      std::string confirmPath = jsonCfg.getConfig("trader", "ConfirmRecordPath").get<std::string>() + "/" + (std::string)user + "/";
      utils::creatFolder(confirmPath);

      CtpTraderInfo traderInfo;

      traderInfo.traderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(conPath.c_str());
      traderInfo.traderSpi = new CtpTraderSpi();
      traderInfo.traderApi->RegisterSpi(traderInfo.traderSpi);
      traderInfo.traderApi->SubscribePrivateTopic(THOST_TERT_QUICK);
      traderInfo.traderApi->SubscribePublicTopic(THOST_TERT_QUICK);
      INFO_LOG("ctp version: %s.", traderInfo.traderApi->GetApiVersion());

      const std::string frontaddr = jsonCfg.getDeepConfig("users", (std::string)user, "FrontAddr").get<std::string>();
      traderInfo.traderApi->RegisterFront(const_cast<char *>(frontaddr.c_str()));
      traderInfo.traderApi->Init();

      auto &globalSem = GlobalSem::getInstance();
      if (globalSem.waitSemBySemName(GlobalSem::loginLogout, 60)) {
        out = false;
        ERROR_LOG("%s", "trader init fail.");
      } else {
        INFO_LOG("%s", "trader init ok.");
      }

      traderInfo.sessionId = traderInfo.traderSpi->sessionId;
      traderInfo.frontId = traderInfo.traderSpi->frontId;
      CtpTraderInfoMap[(std::string)user] = traderInfo;
      std::this_thread::sleep_for(1000ms);
    }
  }
  return out;
}

bool CtpSender::authenticate(void) {
  bool ret = true;
  CThostFtdcReqAuthenticateField pReqAuthenticateField = {0};
  auto &jsonCfg = utils::JsonConfig::getInstance();
  for (auto &item : CtpTraderInfoMap) {
    const std::string authCode = jsonCfg.getDeepConfig("users", item.first, "AuthCode").get<std::string>();
    const std::string appId = jsonCfg.getDeepConfig("users", item.first, "AppID").get<std::string>();
    const std::string userID = jsonCfg.getDeepConfig("users", item.first, "UserID").get<std::string>();
    const std::string brokerID = jsonCfg.getDeepConfig("users", item.first, "BrokerID").get<std::string>();

    strcpy(pReqAuthenticateField.BrokerID, brokerID.c_str());
    strcpy(pReqAuthenticateField.UserID, userID.c_str());
    strcpy(pReqAuthenticateField.AuthCode, authCode.c_str());
    strcpy(pReqAuthenticateField.AppID, appId.c_str());

    auto &globalSem = GlobalSem::getInstance();
    int result = item.second.traderApi->ReqAuthenticate(&pReqAuthenticateField, nRequestID++);
    INFO_LOG("ReqAuthenticate send result is [%d]", result);

    globalSem.waitSemBySemName(GlobalSem::loginLogout);
  }

  INFO_LOG("ReqAuthenticate ok");
  return ret;
}

bool CtpSender::confirm() {
  bool ret = true;
  char buffer[16];
  auto &jsonCfg = utils::JsonConfig::getInstance();

  for (auto &item : CtpTraderInfoMap) {
    std::string confirmDir = jsonCfg.getConfig("trader", "ConfirmRecordPath").get<std::string>();
    std::string confirmFile = confirmDir + "/" + item.first + "/confirmRecord.txt";
    {
      ifstream in(confirmFile);
      if (in.is_open()) {
        in.getline(buffer, 16);
      }
    }

    if (strcmp(buffer, item.second.traderApi->GetTradingDay()) == 0) {
      INFO_LOG("settlement today has been confirmed before!");
    } else {
      CThostFtdcSettlementInfoConfirmField requestMsg{0};
      auto &jsonCfg = utils::JsonConfig::getInstance();
      const std::string brokerID = jsonCfg.getDeepConfig("users", item.first, "BrokerID").get<std::string>();
      const std::string investorID = jsonCfg.getDeepConfig("users", item.first, "InvestorID").get<std::string>();
      strcpy(requestMsg.InvestorID, investorID.c_str());
      strcpy(requestMsg.BrokerID, brokerID.c_str());

      auto &globalSem = GlobalSem::getInstance();
      int result = item.second.traderApi->ReqSettlementInfoConfirm(&requestMsg, nRequestID++);
      INFO_LOG("ReqSettlementInfoConfirm send result is [%d]", result);
      globalSem.waitSemBySemName(GlobalSem::loginLogout);

      ofstream out(confirmFile);
      if (out.is_open()) {
        out << item.second.traderApi->GetTradingDay();
      }
    }
  }
  return ret;
}

bool CtpSender::release() {
  INFO_LOG("Is going to release traderApi.");
  for (auto &item : CtpTraderInfoMap) {
    item.second.traderApi->Release();

    if (item.second.traderSpi) {
      delete item.second.traderSpi;
      item.second.traderSpi = NULL;
    }
  }
}

bool CtpSender::buildorder(utils::OrderContent &content, CThostFtdcInputOrderField &order) {
  memcpy(&order, &defaultOrderField, sizeof(CThostFtdcInputOrderField));

  strcpy(order.OrderRef, content.orderRef.c_str());
  strcpy(order.InstrumentID, content.instrumentID.c_str());
  strcpy(order.ExchangeID, content.exchangeId.c_str());
  order.Direction = content.direction;
  order.LimitPrice = content.limit_price;
  order.VolumeTotalOriginal = content.total_volume;

  order.CombOffsetFlag[0] = content.comboffset;

  if (content.orderType == "limit_LIMIT") {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_GFD;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (content.orderType == "Limit_FAK") {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (content.orderType == "limit_FOK") {
    order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_CV;
  } else if (content.orderType == "AnyPrice_Fok") {
    order.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    order.TimeCondition = THOST_FTDC_TC_GFD;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  } else if (content.orderType == "AnyPrice_Fak") {
    order.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    order.TimeCondition = THOST_FTDC_TC_IOC;
    order.VolumeCondition = THOST_FTDC_VC_AV;
  }
}

bool CtpSender::LossConnection() {
  bool ret = false;
  for (auto &item : CtpTraderInfoMap) {
    if (item.second.traderSpi != nullptr && item.second.traderSpi->frontDisconnected == true) {
      ret = true;
    }
    break;
  }
  return ret;
}
