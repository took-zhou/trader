/*
 * ctpRecer.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include "trader/infra/recer/ctpRecer.h"
#include "common/extern/libgo/libgo/libgo.h"
#include "common/extern/log/log.h"
#include "common/self/fileUtil.h"
#include "common/self/semaphorePart.h"
#include "common/self/utils.h"
#include "trader/infra/define.h"

extern GlobalSem globalSem;
extern co_chan<MsgStruct> ctpMsgChan;

void TraderSpi::OnFrontConnected() {
  INFO_LOG("get OnFrontConnected rsp from ctp");
  INFO_LOG("reConnect:%d.", reConnect);
  if (reConnect++ == 0) {
    std::string semName = "trader_init";
    globalSem.postSemBySemName(semName);
    INFO_LOG("post sem of [%s]", semName.c_str());
  }
}

void TraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                                  bool bIsLast) {
  std::string semName = "trader_reqAuthenticate";
  globalSem.postSemBySemName("trader_reqAuthenticate");
  INFO_LOG("post sem of [%s]", semName.c_str());
}

void TraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspUserLogin";
  msgStruct.ctpMsg = pRspUserLogin;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnRspUserLogin");
  globalSem.waitSemBySemName("OnRspUserLogin");
}

void TraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  std::string semName = "trader_logOut";
  globalSem.postSemBySemName(semName);
  INFO_LOG("post sem of [%s]", semName.c_str());
}

void TraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo,
                                           int nRequestID, bool bIsLast) {
  CThostFtdcSettlementInfoConfirmField *staticSettlementInfoConfirmField = new CThostFtdcSettlementInfoConfirmField;
  *staticSettlementInfoConfirmField = *pSettlementInfoConfirm;
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspSettlementInfoConfirm";
  msgStruct.ctpMsg = pSettlementInfoConfirm;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnRspSettlementInfoConfirm");
  globalSem.waitSemBySemName("OnRspSettlementInfoConfirm");
}

void TraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) {
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnErrRtnOrderInsert";
  msgStruct.ctpMsg = pInputOrder;
  msgStruct.ctpMsgInfo = pRspInfo;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnErrRtnOrderInsert");
  globalSem.waitSemBySemName("OnErrRtnOrderInsert");
}

void TraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspOrderInsert";
  msgStruct.ctpMsg = pInputOrder;
  msgStruct.ctpMsgInfo = pRspInfo;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnRspOrderInsert");
  globalSem.waitSemBySemName("OnRspOrderInsert");
}

void TraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRtnOrder";
  msgStruct.ctpMsg = pOrder;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnRtnOrder");
  globalSem.waitSemBySemName("OnRtnOrder");
}

void TraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRtnTrade";
  msgStruct.ctpMsg = pTrade;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnRtnTrade");
  globalSem.waitSemBySemName("OnRtnTrade");
}

void TraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                                       bool bIsLast) {
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspQryTradingAccount";
  msgStruct.ctpMsg = pTradingAccount;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnRspQryTradingAccount");
  globalSem.waitSemBySemName("OnRspQryTradingAccount");
}

void TraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspQryInstrument";
  msgStruct.ctpMsg = pInstrument;
  msgStruct.bIsLast = bIsLast;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnRspQryInstrument");
  globalSem.waitSemBySemName("OnRspQryInstrument");
}

void TraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                                 bool bIsLast) {
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspOrderAction";
  msgStruct.ctpMsg = pInputOrderAction;
  msgStruct.ctpMsgInfo = pRspInfo;
  msgStruct.bIsLast = bIsLast;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnRspOrderAction");
  globalSem.waitSemBySemName("OnRspOrderAction");
}

void TraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) {
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnErrRtnOrderAction";
  msgStruct.ctpMsg = pOrderAction;
  msgStruct.ctpMsgInfo = pRspInfo;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnErrRtnOrderAction");
  globalSem.waitSemBySemName("OnErrRtnOrderAction");
};

void TraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo,
                                             int nRequestID, bool bIsLast) {
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspQryInstrumentMarginRate";
  msgStruct.ctpMsg = pInstrumentMarginRate;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnRspQryInstrumentMarginRate");
  globalSem.waitSemBySemName("OnRspQryInstrumentMarginRate");
};

void TraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate,
                                                 CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspQryInstrumentCommissionRate";
  msgStruct.ctpMsg = pInstrumentCommissionRate;
  ctpMsgChan << msgStruct;

  globalSem.addOrderSem("OnRspQryInstrumentCommissionRate");
  globalSem.waitSemBySemName("OnRspQryInstrumentCommissionRate");
};
