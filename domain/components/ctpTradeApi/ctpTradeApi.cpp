/*
 * ctpTradeApi.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include <string.h>
#include <array>
#include <string>
#include <thread>

#include "common/self/basetype.h"
#include "common/self/define.h"
#include "common/self/fileUtil.h"
#include "common/self/semaphorePart.h"
#include "common/self/timer.h"
#include "common/self/utils.h"
#include "trader/domain/components/ctpTradeApi/ctpTradeApi.h"
#include "trader/domain/components/order.h"
#include "trader/domain/components/settlementConfirm.h"
#include "trader/infra/recer/ctpRecer.h"
extern GlobalSem globalSem;

CThostFtdcTraderApi *CtpTraderBaseApi::CreateFtdcTraderApi(const char *pszFlowPath) {
  m_pApi = CThostFtdcTraderApi::CreateFtdcTraderApi(pszFlowPath);
  return m_pApi;
}

const char *CtpTraderBaseApi::GetTradingDay() { return m_pApi->GetTradingDay(); };

void CtpTraderBaseApi::RegisterFront(char *pszFrontAddress) {
  m_pApi->RegisterFront(pszFrontAddress);
  INFO_LOG("RegisterFront [%s] ok", pszFrontAddress);
}

void CtpTraderBaseApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) {
  m_pApi->SubscribePrivateTopic(nResumeType);
  INFO_LOG("SubscribePrivateTopic ok sub type is [%d]", (int)nResumeType);
}

void CtpTraderBaseApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) {
  m_pApi->SubscribePublicTopic(nResumeType);
  INFO_LOG("SubscribePublicTopic ok sub type is [%d]", (int)nResumeType);
}

void CtpTraderBaseApi::RegisterSpi(CThostFtdcTraderSpi *pSpi) {
  m_pApi->RegisterSpi(pSpi);
  INFO_LOG("RegisterSpi ok");
}

int CtpTraderBaseApi::ReqOrderAction(OrderContent *orderContent, char actionFlag) {
  CThostFtdcInputOrderActionField orderActionReq{0};
  strcpy(orderActionReq.OrderRef, orderContent->orderRef.c_str());
  orderActionReq.FrontID = orderContent->frontId;
  orderActionReq.SessionID = orderContent->sessionId;
  strcpy(orderActionReq.InstrumentID, orderContent->instrumentID.c_str());
  strcpy(orderActionReq.InvestorID, orderContent->investorId.c_str());
  strcpy(orderActionReq.UserID, orderContent->userId.c_str());
  orderActionReq.ActionFlag = actionFlag;
  int result = m_pApi->ReqOrderAction(&orderActionReq, requestIdBuildAlg());
  INFO_LOG("ReqOrderAction send result is [%d]", result);
  return result;
}

int CtpTraderBaseApi::ReqSettlementInfoConfirm() {
  CThostFtdcSettlementInfoConfirmField requestMsg{0};
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();
  const std::string investorID = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();
  strcpy(requestMsg.InvestorID, investorID.c_str());
  strcpy(requestMsg.BrokerID, brokerID.c_str());

  std::string semName = "trader_ReqSettlementInfoConfirm";
  globalSem.addOrderSem(semName);
  int result = m_pApi->ReqSettlementInfoConfirm(&requestMsg, requestIdBuildAlg());
  INFO_LOG("ReqSettlementInfoConfirm send result is [%d]", result);
  return result;
}

int CtpTraderBaseApi::ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder) {
  int result = m_pApi->ReqOrderInsert(pInputOrder, requestIdBuildAlg());
  INFO_LOG("ReqOrderInsert send result is [%d], orderRef is[%s]", result, pInputOrder->OrderRef);
  return result;
}

int CtpTraderBaseApi::requestIdBuildAlg() {
  if (requestID == S32_MAX) {
    requestID = 1;
    return requestID;
  }
  requestID++;
  return requestID;
}

int CtpTraderBaseApi::ReqAuthenticate() {
  CThostFtdcReqAuthenticateField pReqAuthenticateField = {0};
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string authCode = jsonCfg.getDeepConfig("users", username, "AuthCode").get<std::string>();
  const std::string appId = jsonCfg.getDeepConfig("users", username, "AppID").get<std::string>();
  const std::string userID = jsonCfg.getDeepConfig("users", username, "UserID").get<std::string>();
  const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();

  strcpy(pReqAuthenticateField.BrokerID, brokerID.c_str());
  strcpy(pReqAuthenticateField.UserID, userID.c_str());
  // strcpy(a.UserProductInfo, "");
  strcpy(pReqAuthenticateField.AuthCode, authCode.c_str());
  strcpy(pReqAuthenticateField.AppID, appId.c_str());

  std::string semName = "trader_reqAuthenticate";
  globalSem.addOrderSem(semName);
  int result = m_pApi->ReqAuthenticate(&pReqAuthenticateField, requestIdBuildAlg());
  INFO_LOG("ReqAuthenticate send result is [%d]", result);
  return result;
}

int CtpTraderBaseApi::ReqUserLogin() {
  CThostFtdcReqUserLoginField reqUserLogin{0};
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string userID = jsonCfg.getDeepConfig("users", username, "UserID").get<std::string>();
  const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();
  const std::string passWord = jsonCfg.getDeepConfig("users", username, "Password").get<std::string>();
  strcpy(reqUserLogin.BrokerID, brokerID.c_str());
  strcpy(reqUserLogin.UserID, userID.c_str());
  strcpy(reqUserLogin.Password, passWord.c_str());

  std::string semName = "trader_logIn";
  globalSem.addOrderSem(semName);
  int result = m_pApi->ReqUserLogin(&reqUserLogin, requestIdBuildAlg());
  INFO_LOG("ReqUserLogin send result is [%d]", result);
  return result;
}

int CtpTraderBaseApi::ReqUserLogout() {
  CThostFtdcUserLogoutField logOutField{0};
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string userID = jsonCfg.getDeepConfig("users", username, "UserID").get<std::string>();
  const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();
  strcpy(logOutField.BrokerID, brokerID.c_str());
  strcpy(logOutField.UserID, userID.c_str());

  std::string semName = "trader_logOut";
  globalSem.addOrderSem(semName);
  int result = m_pApi->ReqUserLogout(&logOutField, requestIdBuildAlg());
  INFO_LOG("ReqUserLogout send result is [%d]", result);
  return result;
}

int CtpTraderBaseApi::ReqQryInstrumentMarginRate(utils::InstrumtntID ins_exch) {
  CThostFtdcQryInstrumentMarginRateField marginRateField{0};

  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string investorID = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();
  const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();

  strcpy(marginRateField.BrokerID, brokerID.c_str());
  strcpy(marginRateField.InvestorID, investorID.c_str());
  strcpy(marginRateField.ExchangeID, ins_exch.exch.c_str());
  strcpy(marginRateField.InstrumentID, ins_exch.ins.c_str());
  marginRateField.HedgeFlag = THOST_FTDC_HF_Speculation;

  int result = m_pApi->ReqQryInstrumentMarginRate(&marginRateField, requestIdBuildAlg());
  INFO_LOG("ReqQryInstrumentMarginRate send result is [%d]", result);

  if (result == 0) {
    std::string semName = "margin_rate";
    globalSem.addOrderSem(semName);
  }

  return result;
}

int CtpTraderBaseApi::ReqQryInstrumentCommissionRate(utils::InstrumtntID ins_exch) {
  CThostFtdcQryInstrumentCommissionRateField commissonRateField{0};
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string investorID = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();
  const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();
  strcpy(commissonRateField.BrokerID, brokerID.c_str());
  strcpy(commissonRateField.InvestorID, investorID.c_str());
  strcpy(commissonRateField.ExchangeID, ins_exch.exch.c_str());
  strcpy(commissonRateField.InstrumentID, ins_exch.ins.c_str());

  int result = m_pApi->ReqQryInstrumentCommissionRate(&commissonRateField, requestIdBuildAlg());
  INFO_LOG("ReqQryInstrumentCommissionRate send result is [%d]", result);

  if (result == 0) {
    std::string semName = "commission_rate";
    globalSem.addOrderSem(semName);
  }

  return result;
}

int CtpTraderBaseApi::ReqQryInstrument(utils::InstrumtntID ins_exch) {
  CThostFtdcQryInstrumentField requestMsg{0};
  std::strcpy(requestMsg.ExchangeID, ins_exch.exch.c_str());
  std::strcpy(requestMsg.InstrumentID, ins_exch.ins.c_str());

  std::string semName = "";
  if (ins_exch.exch == "" && ins_exch.ins == "") {
    semName = "trader_ReqQryInstrument_ALL";
  } else {
    semName = "trader_ReqQryInstrument_Single";
  }

  globalSem.addOrderSem(semName);
  int result = m_pApi->ReqQryInstrument(&requestMsg, requestIdBuildAlg());
  INFO_LOG("ReqQryInstrument send result is [%d]", result);
  return result;
}

int CtpTraderBaseApi::ReqQryTradingAccount() {
  CThostFtdcQryTradingAccountField requestMsg{0};
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string brokerID = jsonCfg.getDeepConfig("users", username, "BrokerID").get<std::string>();
  const std::string investorID = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();
  strcpy(requestMsg.InvestorID, investorID.c_str());
  strcpy(requestMsg.BrokerID, brokerID.c_str());
  strcpy(requestMsg.CurrencyID, "CNY");

  int result = m_pApi->ReqQryTradingAccount(&requestMsg, requestIdBuildAlg());

  if (result == 0) {
    std::string semName = "trader_ReqQryTradingAccount";
    globalSem.addOrderSem(semName);
  }

  return result;
}

void CtpTraderBaseApi::Release() {
  if (m_pApi) {
    m_pApi->Release();
    m_pApi = NULL;
  }
  return;
}

void CtpTraderBaseApi::Init() {
  std::string semName = "trader_init";
  globalSem.addOrderSem(semName);
  m_pApi->Init();
  INFO_LOG("m_pApi->Init send ok!");
}

bool CtpTraderApi::init() {
  bool out = false;
  INFO_LOG("begin CtpTraderApi init");

  traderApi = new CtpTraderBaseApi;  //新建tradeAPI
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string conPath = jsonCfg.getConfig("trader", "ConPath").get<std::string>();
  utils::creatFolder(conPath);
  traderApi->CreateFtdcTraderApi(conPath.c_str());  //设置.con的保存位置
  traderSpi = new TraderSpi();

  traderApi->RegisterSpi(traderSpi);                   //注册回调类
  traderApi->SubscribePrivateTopic(THOST_TERT_QUICK);  //订阅
  traderApi->SubscribePublicTopic(THOST_TERT_QUICK);   //订阅

  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string frontaddr = jsonCfg.getDeepConfig("users", username, "FrontAddr").get<std::string>();

  traderApi->RegisterFront(const_cast<char *>(frontaddr.c_str()));  //注册前置地址

  traderApi->Init();  // connect to ctp
  std::string semName = "trader_init";

  if (globalSem.waitSemBySemName(semName, 10)) {
    out = false;
    login_state = ERROR_STATE;
    ERROR_LOG("%s", "trader init fail.");
  } else {
    out = true;
    INFO_LOG("%s", "trader init ok.");
  }
  globalSem.delOrderSem(semName);

  return out;
}

void CtpTraderApi::release() {
  INFO_LOG("Is going to release traderApi.");
  traderApi->Release();
  delete traderApi;
  traderApi = nullptr;

  // 释放UserSpi实例
  if (traderSpi) {
    delete traderSpi;
    traderSpi = NULL;
  }
}

bool CtpTraderApi::logIn() {
  traderApi->ReqAuthenticate();

  std::string semName = "trader_reqAuthenticate";
  globalSem.waitSemBySemName(semName);
  globalSem.delOrderSem(semName);

  INFO_LOG("ReqAuthenticate ok");

  traderApi->ReqUserLogin();
  semName = "trader_logIn";
  globalSem.waitSemBySemName(semName);
  globalSem.delOrderSem(semName);

  INFO_LOG("login ctp ok!");

  std::string tradingDay = traderApi->GetTradingDay();
  INFO_LOG("trading day:%s", tradingDay.c_str());
  if (this->ROLE(SettlementConfirm).confirm(tradingDay)) {
    INFO_LOG("SettlementConfirm confirm success!");
  } else {
    ERROR_LOG("SettlementConfirm confirm fail!");
  }

  login_state = LOGIN_STATE;
  return true;
}

void CtpTraderApi::logOut() {
  if (traderApi != nullptr) {
    traderApi->ReqUserLogout();

    std::string semName = "trader_logOut";
    globalSem.waitSemBySemName(semName, 10);
    globalSem.delOrderSem(semName);

    release();
    INFO_LOG("ctp log out ok!");
  }

  login_state = LOGOUT_STATE;
}

CtpTraderApi::CtpTraderApi() {
  // static CtpTraderBaseApi ctpTraderBaseApi;
  // traderApi = &ctpTraderBaseApi;
}

TRADER_LOGIN_STATE CtpTraderApi::getTraderLoginState(void) { return login_state; }

void CtpTraderApi::runLogInAndLogOutAlg() {
  while (1) {
    if (ROLE(TraderTimeState).output.status == LOGIN_TIME && login_state == LOGOUT_STATE) {
      if (this->init()) {
        this->logIn();
      } else {
        this->release();
      }
    } else if ((ROLE(TraderTimeState).output.status == LOGOUT_TIME || (traderSpi != nullptr && traderSpi->reConnect > 1)) &&
               login_state != LOGOUT_STATE) {
      this->logOut();
    }
    std::this_thread::sleep_for(1000ms);
  }
}
