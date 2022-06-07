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

// std::mutex m;

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
  MSG_LOG("%s", "<OnRspAuthenticate>");
  if (pRspAuthenticateField) {
    PURE_LOG("\tBrokerID [%s]", pRspAuthenticateField->BrokerID);
    PURE_LOG("\tUserID [%s]", pRspAuthenticateField->UserID);
    PURE_LOG("\tUserProductInfo [%s]", pRspAuthenticateField->UserProductInfo);
    PURE_LOG("\tAppID [%s]", pRspAuthenticateField->AppID);
    PURE_LOG("\tAppType [%c]", pRspAuthenticateField->AppType);
  }
  if (pRspInfo) {
    TThostFtdcErrorMsgType msg;
    utils::gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
    PURE_LOG("\tErrorMsg [%s]", msg);
    PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
  }
  PURE_LOG("\tnRequestID [%d]", nRequestID);
  PURE_LOG("\tbIsLast [%d]", bIsLast);
  MSG_LOG("%s", "</OnRspAuthenticate>");
  INFO_LOG("%s", "ReqAuthenticate OK");

  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspAuthenticate";
  ctpMsgChan << msgStruct;
}

void TraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  MSG_LOG("%s", "<OnRspUserLogin>");
  if (pRspUserLogin) {
    PURE_LOG("\tTradingDay [%s]", pRspUserLogin->TradingDay);
    PURE_LOG("\tLoginTime [%s]", pRspUserLogin->LoginTime);
    PURE_LOG("\tBrokerID [%s]", pRspUserLogin->BrokerID);
    PURE_LOG("\tUserID [%s]", pRspUserLogin->UserID);
    PURE_LOG("\tSystemName [%s]", pRspUserLogin->SystemName);
    PURE_LOG("\tMaxOrderRef [%s]", pRspUserLogin->MaxOrderRef);
    PURE_LOG("\tSHFETime [%s]", pRspUserLogin->SHFETime);
    PURE_LOG("\tDCETime [%s]", pRspUserLogin->DCETime);
    PURE_LOG("\tCZCETime [%s]", pRspUserLogin->CZCETime);
    PURE_LOG("\tFFEXTime [%s]", pRspUserLogin->FFEXTime);
    PURE_LOG("\tINETime [%s]", pRspUserLogin->INETime);
    PURE_LOG("\tFrontID [%d]", pRspUserLogin->FrontID);
    PURE_LOG("\tSessionID [%d]", pRspUserLogin->SessionID);
  }
  TThostFtdcErrorMsgType msg;
  utils::gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
  if (pRspInfo) {
    PURE_LOG("\tErrorMsg [%s]", msg);
    // TThostFtdcErrorMsgType msg;gbk2utf8(pRspInfo->ErrorMsg, msg,
    // sizeof(msg));PURE_LOG("\tErrorMsg [%s]", msg);
    PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
  }
  PURE_LOG("\tErrorMsg [%s]", msg);
  PURE_LOG("\tnRequestID [%d]", nRequestID);
  PURE_LOG("\tbIsLast [%d]", bIsLast);
  MSG_LOG("%s", "</OnRspUserLogin>");

  CThostFtdcRspUserLoginField *staticRspUserLoginField = new CThostFtdcRspUserLoginField;
  *staticRspUserLoginField = *pRspUserLogin;
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspUserLogin";
  msgStruct.ctpMsg = static_cast<void *>(staticRspUserLoginField);
  ctpMsgChan << msgStruct;
}

void TraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  INFO_LOG("<OnRspUserLogout>");
  if (pUserLogout) {
    INFO_LOG("\tBrokerID [%s]", pUserLogout->BrokerID);
    INFO_LOG("\tUserID [%s]", pUserLogout->UserID);
  }
  if (pRspInfo) {
    TThostFtdcErrorMsgType msg;
    utils::gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
    INFO_LOG("\tErrorMsg [%s]", msg);
    INFO_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
  }
  INFO_LOG("\tnRequestID [%d]", nRequestID);
  INFO_LOG("\tbIsLast [%d]", bIsLast);
  INFO_LOG("</OnRspUserLogout>");
  std::string semName = "trader_logOut";
  globalSem.postSemBySemName(semName);
  INFO_LOG("post sem of [%s]", semName.c_str());
}

void TraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo,
                                           int nRequestID, bool bIsLast) {
  MSG_LOG("%s", "<OnRspSettlementInfoConfirm>");
  if (pSettlementInfoConfirm) {
    PURE_LOG("\tBrokerID [%s]", pSettlementInfoConfirm->BrokerID);
    PURE_LOG("\tInvestorID [%s]", pSettlementInfoConfirm->InvestorID);
    PURE_LOG("\tConfirmDate [%s]", pSettlementInfoConfirm->ConfirmDate);
    PURE_LOG("\tConfirmTime [%s]", pSettlementInfoConfirm->ConfirmTime);
    PURE_LOG("\tAccountID [%s]", pSettlementInfoConfirm->AccountID);
    PURE_LOG("\tCurrencyID [%s]", pSettlementInfoConfirm->CurrencyID);
    PURE_LOG("\tSettlementID [%d]", pSettlementInfoConfirm->SettlementID);
  }
  if (pRspInfo) {
    TThostFtdcErrorMsgType msg;
    utils::gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
    PURE_LOG("\tErrorMsg [%s]", msg);
    PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
  }
  PURE_LOG("\tnRequestID [%d]", nRequestID);
  PURE_LOG("\tbIsLast [%d]", bIsLast);
  PURE_LOG("</OnRspSettlementInfoConfirm>");

  CThostFtdcSettlementInfoConfirmField *staticSettlementInfoConfirmField = new CThostFtdcSettlementInfoConfirmField;
  *staticSettlementInfoConfirmField = *pSettlementInfoConfirm;
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspSettlementInfoConfirm";
  msgStruct.ctpMsg = static_cast<void *>(staticSettlementInfoConfirmField);
  ctpMsgChan << msgStruct;
}

void TraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) {
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string investorID = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();

  if (pInputOrder && strcmp(pInputOrder->InvestorID, investorID.c_str()) != 0) {
    return;
  } else {
    PURE_LOG("<OnErrRtnOrderInsert>");
    // if (pInputOrder)
    // {
    // PURE_LOG("\tBrokerID [%s]", pInputOrder->BrokerID);
    // PURE_LOG("\tInvestorID [%s]", pInputOrder->InvestorID);
    // PURE_LOG("\tInstrumentID [%s]", pInputOrder->InstrumentID);
    // PURE_LOG("\tOrderRef [%s]", pInputOrder->OrderRef);
    // PURE_LOG("\tUserID [%s]", pInputOrder->UserID);
    // PURE_LOG("\tCombOffsetFlag [%s]", pInputOrder->CombOffsetFlag);
    // PURE_LOG("\tCombHedgeFlag [%s]", pInputOrder->CombHedgeFlag);
    // PURE_LOG("\tGTDDate [%s]", pInputOrder->GTDDate);
    // PURE_LOG("\tBusinessUnit [%s]", pInputOrder->BusinessUnit);
    // PURE_LOG("\tExchangeID [%s]", pInputOrder->ExchangeID);
    // PURE_LOG("\tInvestUnitID [%s]", pInputOrder->InvestUnitID);
    // PURE_LOG("\tAccountID [%s]", pInputOrder->AccountID);
    // PURE_LOG("\tCurrencyID [%s]", pInputOrder->CurrencyID);
    // PURE_LOG("\tClientID [%s]", pInputOrder->ClientID);
    // PURE_LOG("\tIPAddress [%s]", pInputOrder->IPAddress);
    // PURE_LOG("\tMacAddress [%s]", pInputOrder->MacAddress);
    // PURE_LOG("\tVolumeTotalOriginal [%d]", pInputOrder->VolumeTotalOriginal);
    // PURE_LOG("\tMinVolume [%d]", pInputOrder->MinVolume);
    // PURE_LOG("\tIsAutoSuspend [%d]", pInputOrder->IsAutoSuspend);
    // PURE_LOG("\tRequestID [%d]", pInputOrder->RequestID);
    // PURE_LOG("\tUserForceClose [%d]", pInputOrder->UserForceClose);
    // PURE_LOG("\tIsSwapOrder [%d]", pInputOrder->IsSwapOrder);
    // PURE_LOG("\tOrderPriceType [%c]", pInputOrder->OrderPriceType);
    // PURE_LOG("\tDirection [%c]", pInputOrder->Direction);
    // PURE_LOG("\tTimeCondition [%c]", pInputOrder->TimeCondition);
    // PURE_LOG("\tVolumeCondition [%c]", pInputOrder->VolumeCondition);
    // PURE_LOG("\tContingentCondition [%c]", pInputOrder->ContingentCondition);
    // PURE_LOG("\tForceCloseReason [%c]", pInputOrder->ForceCloseReason);
    // PURE_LOG("\tLimitPrice [%.8lf]", pInputOrder->LimitPrice);
    // PURE_LOG("\tStopPrice [%.8lf]", pInputOrder->StopPrice);
    // }
    if (pRspInfo) {
      TThostFtdcErrorMsgType msg;
      utils::gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
      PURE_LOG("\tErrorMsg [%s]", msg);
      PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
    }
    PURE_LOG("</OnErrRtnOrderInsert>");
  }

  CThostFtdcInputOrderField *staticInputOrderField = new CThostFtdcInputOrderField;
  CThostFtdcRspInfoField *staticRspInfoField = new CThostFtdcRspInfoField;
  *staticInputOrderField = *pInputOrder;
  *staticRspInfoField = *pRspInfo;
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnErrRtnOrderInsert";
  msgStruct.ctpMsg = static_cast<void *>(staticInputOrderField);
  msgStruct.ctpMsgInfo = static_cast<void *>(staticRspInfoField);
  ctpMsgChan << msgStruct;
}

void TraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string investorID = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();

  if (pInputOrder && strcmp(pInputOrder->InvestorID, investorID.c_str()) != 0) {
    return;
  } else {
    PURE_LOG("<OnRspOrderInsert>");
    // if (pInputOrder)
    // {
    // PURE_LOG("\tBrokerID [%s]", pInputOrder->BrokerID);
    // PURE_LOG("\tInvestorID [%s]", pInputOrder->InvestorID);
    // PURE_LOG("\tInstrumentID [%s]", pInputOrder->InstrumentID);
    // PURE_LOG("\tOrderRef [%s]", pInputOrder->OrderRef);
    // PURE_LOG("\tUserID [%s]", pInputOrder->UserID);
    // PURE_LOG("\tCombOffsetFlag [%s]", pInputOrder->CombOffsetFlag);
    // PURE_LOG("\tCombHedgeFlag [%s]", pInputOrder->CombHedgeFlag);
    // PURE_LOG("\tGTDDate [%s]", pInputOrder->GTDDate);
    // PURE_LOG("\tBusinessUnit [%s]", pInputOrder->BusinessUnit);
    // PURE_LOG("\tExchangeID [%s]", pInputOrder->ExchangeID);
    // PURE_LOG("\tInvestUnitID [%s]", pInputOrder->InvestUnitID);
    // PURE_LOG("\tAccountID [%s]", pInputOrder->AccountID);
    // PURE_LOG("\tCurrencyID [%s]", pInputOrder->CurrencyID);
    // PURE_LOG("\tClientID [%s]", pInputOrder->ClientID);
    // PURE_LOG("\tIPAddress [%s]", pInputOrder->IPAddress);
    // PURE_LOG("\tMacAddress [%s]", pInputOrder->MacAddress);
    // PURE_LOG("\tVolumeTotalOriginal [%d]", pInputOrder->VolumeTotalOriginal);
    // PURE_LOG("\tMinVolume [%d]", pInputOrder->MinVolume);
    // PURE_LOG("\tIsAutoSuspend [%d]", pInputOrder->IsAutoSuspend);
    // PURE_LOG("\tRequestID [%d]", pInputOrder->RequestID);
    // PURE_LOG("\tUserForceClose [%d]", pInputOrder->UserForceClose);
    // PURE_LOG("\tIsSwapOrder [%d]", pInputOrder->IsSwapOrder);
    // PURE_LOG("\tOrderPriceType [%c]", pInputOrder->OrderPriceType);
    // PURE_LOG("\tDirection [%c]", pInputOrder->Direction);
    // PURE_LOG("\tTimeCondition [%c]", pInputOrder->TimeCondition);
    // PURE_LOG("\tVolumeCondition [%c]", pInputOrder->VolumeCondition);
    // PURE_LOG("\tContingentCondition [%c]", pInputOrder->ContingentCondition);
    // PURE_LOG("\tForceCloseReason [%c]", pInputOrder->ForceCloseReason);
    // PURE_LOG("\tLimitPrice [%.8lf]", pInputOrder->LimitPrice);
    // PURE_LOG("\tStopPrice [%.8lf]", pInputOrder->StopPrice);
    // }
    if (pRspInfo) {
      TThostFtdcErrorMsgType msg;
      utils::gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
      PURE_LOG("\tErrorMsg [%s]", msg);

      PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
    }
    PURE_LOG("\tnRequestID [%d]", nRequestID);
    PURE_LOG("\tbIsLast [%d]", bIsLast);
    PURE_LOG("</OnRspOrderInsert>");
  }

  CThostFtdcInputOrderField *staticInputOrderField = new CThostFtdcInputOrderField;
  CThostFtdcRspInfoField *staticRspInfoField = new CThostFtdcRspInfoField;

  *staticInputOrderField = *pInputOrder;
  *staticRspInfoField = *pRspInfo;
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspOrderInsert";
  msgStruct.ctpMsg = static_cast<void *>(staticInputOrderField);
  msgStruct.ctpMsgInfo = static_cast<void *>(staticRspInfoField);
  ctpMsgChan << msgStruct;
}

void TraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string username = jsonCfg.getConfig("common", "user").get<std::string>();
  const std::string investorID = jsonCfg.getDeepConfig("users", username, "InvestorID").get<std::string>();

  if (pOrder && strcmp(pOrder->InvestorID, investorID.c_str()) != 0) {
    return;
  } else {
    // MSG_LOG("%s","******<OnRtnOrder>******");
    if (pOrder) {
      // PURE_LOG("\tBrokerID [%s]", pOrder->BrokerID);
      // PURE_LOG("\tInvestorID [%s]", pOrder->InvestorID);
      // PURE_LOG("\tInstrumentID [%s]", pOrder->InstrumentID);

      // PURE_LOG("\tOrderRef [%s]", pOrder->OrderRef);
      // PURE_LOG("\tUserID [%s]", pOrder->UserID);
      // PURE_LOG("\tCombOffsetFlag [%s]", pOrder->CombOffsetFlag);

      // PURE_LOG("\tCombHedgeFlag [%s]", pOrder->CombHedgeFlag);
      // PURE_LOG("\tGTDDate [%s]", pOrder->GTDDate);
      // PURE_LOG("\tBusinessUnit [%s]", pOrder->BusinessUnit);
      // PURE_LOG("\tOrderLocalID [%s]", pOrder->OrderLocalID);
      // PURE_LOG("\tExchangeID [%s]", pOrder->ExchangeID);

      // PURE_LOG("\tParticipantID [%s]", pOrder->ParticipantID);
      // PURE_LOG("\tClientID [%s]", pOrder->ClientID);
      // PURE_LOG("\tExchangeInstID [%s]", pOrder->ExchangeInstID);
      // PURE_LOG("\tTraderID [%s]", pOrder->TraderID);
      // PURE_LOG("\tTradingDay [%s]", pOrder->TradingDay);

      // PURE_LOG("\tOrderSysID [%s]", pOrder->OrderSysID);

      // PURE_LOG("\tInsertDate [%s]", pOrder->InsertDate);
      // PURE_LOG("\tInsertTime [%s]", pOrder->InsertTime);

      // PURE_LOG("\tActiveTime [%s]", pOrder->ActiveTime);
      // PURE_LOG("\tSuspendTime [%s]", pOrder->SuspendTime);
      // PURE_LOG("\tUpdateTime [%s]", pOrder->UpdateTime);
      // PURE_LOG("\tCancelTime [%s]", pOrder->CancelTime);
      // PURE_LOG("\tActiveTraderID [%s]", pOrder->ActiveTraderID);
      // PURE_LOG("\tClearingPartID [%s]", pOrder->ClearingPartID);
      // PURE_LOG("\tUserProductInfo [%s]", pOrder->UserProductInfo);

      // TThostFtdcErrorMsgType msg;
      // utils::gbk2utf8(pOrder->StatusMsg, msg, sizeof(msg));
      // PURE_LOG("\tStatusMsg [%s]", msg);

      // PURE_LOG("\tActiveUserID [%s]", pOrder->ActiveUserID);
      // PURE_LOG("\tRelativeOrderSysID [%s]", pOrder->RelativeOrderSysID);
      // PURE_LOG("\tBranchID [%s]", pOrder->BranchID);
      // PURE_LOG("\tInvestUnitID [%s]", pOrder->InvestUnitID);
      // PURE_LOG("\tAccountID [%s]", pOrder->AccountID);
      // PURE_LOG("\tCurrencyID [%s]", pOrder->CurrencyID);
      // PURE_LOG("\tIPAddress [%s]", pOrder->IPAddress);
      // PURE_LOG("\tMacAddress [%s]", pOrder->MacAddress);
      // PURE_LOG("\tVolumeTotalOriginal [%d]", pOrder->VolumeTotalOriginal);

      // PURE_LOG("\tMinVolume [%d]", pOrder->MinVolume);
      // PURE_LOG("\tIsAutoSuspend [%d]", pOrder->IsAutoSuspend);
      // PURE_LOG("\tRequestID [%d]", pOrder->RequestID);
      // PURE_LOG("\tInstallID [%d]", pOrder->InstallID);
      // PURE_LOG("\tNotifySequence [%d]", pOrder->NotifySequence);
      // PURE_LOG("\tSettlementID [%d]", pOrder->SettlementID);
      // PURE_LOG("\tVolumeTraded [%d]", pOrder->VolumeTraded);
      // PURE_LOG("\tVolumeTotal [%d]", pOrder->VolumeTotal);
      // PURE_LOG("\tSequenceNo [%d]", pOrder->SequenceNo);
      // PURE_LOG("\tFrontID [%d]", pOrder->FrontID);
      // PURE_LOG("\tSessionID [%d]", pOrder->SessionID);

      // PURE_LOG("\tUserForceClose [%d]", pOrder->UserForceClose);
      // PURE_LOG("\tBrokerOrderSeq [%d]", pOrder->BrokerOrderSeq);
      // PURE_LOG("\tZCETotalTradedVolume [%d]", pOrder->ZCETotalTradedVolume);
      // PURE_LOG("\tIsSwapOrder [%d]", pOrder->IsSwapOrder);
      // PURE_LOG("\tOrderPriceType [%c]", pOrder->OrderPriceType);
      // PURE_LOG("\tDirection [%c]", pOrder->Direction);

      // PURE_LOG("\tTimeCondition [%c]", pOrder->TimeCondition);
      // PURE_LOG("\tVolumeCondition [%c]", pOrder->VolumeCondition);
      // PURE_LOG("\tContingentCondition [%c]", pOrder->ContingentCondition);
      // PURE_LOG("\tForceCloseReason [%c]", pOrder->ForceCloseReason);
      // PURE_LOG("\tOrderSubmitStatus [%c]", pOrder->OrderSubmitStatus);
      // PURE_LOG("\tOrderSource [%c]", pOrder->OrderSource);
      // PURE_LOG("\tOrderStatus [%c]", pOrder->OrderStatus);
      // PURE_LOG("\tOrderType [%c]", pOrder->OrderType);
      // PURE_LOG("\tLimitPrice [%.8lf]", pOrder->LimitPrice);

      // PURE_LOG("\tStopPrice [%.8lf]", pOrder->StopPrice);
    }
    // MSG_LOG("%s","******</OnRtnOrder>******");
  }

  CThostFtdcOrderField *staticOrderField = new CThostFtdcOrderField;
  *staticOrderField = *pOrder;
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRtnOrder";
  msgStruct.ctpMsg = static_cast<void *>(staticOrderField);
  ctpMsgChan << msgStruct;
}

void TraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
  // INFO_LOG("******<OnRtnTrade>******");
  // if (pTrade)
  // {
  //     PURE_LOG("\tBrokerID [%s]", pTrade->BrokerID);
  //     PURE_LOG("\tInvestorID [%s]", pTrade->InvestorID);
  //     PURE_LOG("\tInstrumentID [%s]", pTrade->InstrumentID);
  //     PURE_LOG("\tOrderRef [%s]", pTrade->OrderRef);
  //     PURE_LOG("\tUserID [%s]", pTrade->UserID);
  //     PURE_LOG("\tExchangeID [%s]", pTrade->ExchangeID);
  //     PURE_LOG("\tTradeID [%s]", pTrade->TradeID);
  //     PURE_LOG("\tOrderSysID [%s]", pTrade->OrderSysID);
  //     PURE_LOG("\tParticipantID [%s]", pTrade->ParticipantID);
  //     PURE_LOG("\tClientID [%s]", pTrade->ClientID);
  //     PURE_LOG("\tExchangeInstID [%s]", pTrade->ExchangeInstID);
  //     PURE_LOG("\tTradeDate [%s]", pTrade->TradeDate);
  //     PURE_LOG("\tTradeTime [%s]", pTrade->TradeTime);
  //     PURE_LOG("\tTraderID [%s]", pTrade->TraderID);
  //     PURE_LOG("\tOrderLocalID [%s]", pTrade->OrderLocalID);
  //     PURE_LOG("\tClearingPartID [%s]", pTrade->ClearingPartID);
  //     PURE_LOG("\tBusinessUnit [%s]", pTrade->BusinessUnit);
  //     PURE_LOG("\tTradingDay [%s]", pTrade->TradingDay);
  //     PURE_LOG("\tInvestUnitID [%s]", pTrade->InvestUnitID);
  //     PURE_LOG("\tVolume [%d]", pTrade->Volume);
  //     PURE_LOG("\tSequenceNo [%d]", pTrade->SequenceNo);
  //     PURE_LOG("\tSettlementID [%d]", pTrade->SettlementID);
  //     PURE_LOG("\tBrokerOrderSeq [%d]", pTrade->BrokerOrderSeq);
  //     PURE_LOG("\tDirection [%c]", pTrade->Direction);
  //     PURE_LOG("\tTradingRole [%c]", pTrade->TradingRole);
  //     PURE_LOG("\tOffsetFlag [%c]", pTrade->OffsetFlag);
  //     PURE_LOG("\tHedgeFlag [%c]", pTrade->HedgeFlag);
  //     PURE_LOG("\tTradeType [%c]", pTrade->TradeType);
  //     PURE_LOG("\tPriceSource [%c]", pTrade->PriceSource);
  //     PURE_LOG("\tTradeSource [%c]", pTrade->TradeSource);
  //     PURE_LOG("\tPrice [%.8lf]", pTrade->Price);
  // }
  // INFO_LOG("******</OnRtnTrade>******");

  CThostFtdcTradeField *staticTradeField = new CThostFtdcTradeField;
  *staticTradeField = *pTrade;
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRtnTrade";
  msgStruct.ctpMsg = static_cast<void *>(staticTradeField);
  ctpMsgChan << msgStruct;
}

void TraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                                       bool bIsLast) {
  // PURE_LOG("<OnRspQryTradingAccount>");
  // if (pTradingAccount)
  // {
  //     PURE_LOG("\tBrokerID [%s]", pTradingAccount->BrokerID);
  //     PURE_LOG("\tAccountID [%s]", pTradingAccount->AccountID);
  //     PURE_LOG("\tTradingDay [%s]", pTradingAccount->TradingDay);
  //     PURE_LOG("\tCurrencyID [%s]", pTradingAccount->CurrencyID);
  //     PURE_LOG("\tSettlementID [%d]", pTradingAccount->SettlementID);
  //     PURE_LOG("\tBizType [%c]", pTradingAccount->BizType);
  //     PURE_LOG("\tPreMortgage [%.8lf]", pTradingAccount->PreMortgage);
  //     PURE_LOG("\tPreCredit [%.8lf]", pTradingAccount->PreCredit);
  //     PURE_LOG("\tPreDeposit [%.8lf]", pTradingAccount->PreDeposit);
  //     PURE_LOG("\tPreBalance [%.8lf]", pTradingAccount->PreBalance);
  //     PURE_LOG("\tPreMargin [%.8lf]", pTradingAccount->PreMargin);
  //     PURE_LOG("\tInterestBase [%.8lf]", pTradingAccount->InterestBase);
  //     PURE_LOG("\tInterest [%.8lf]", pTradingAccount->Interest);
  //     PURE_LOG("\tDeposit [%.8lf]", pTradingAccount->Deposit);
  //     PURE_LOG("\tWithdraw [%.8lf]", pTradingAccount->Withdraw);
  //     PURE_LOG("\tFrozenMargin [%.8lf]", pTradingAccount->FrozenMargin);
  //     PURE_LOG("\tFrozenCash [%.8lf]", pTradingAccount->FrozenCash);
  //     PURE_LOG("\tFrozenCommission [%.8lf]",
  //     pTradingAccount->FrozenCommission); PURE_LOG("\tCurrMargin [%.8lf]",
  //     pTradingAccount->CurrMargin); PURE_LOG("\tCashIn [%.8lf]",
  //     pTradingAccount->CashIn); PURE_LOG("\tCommission [%.8lf]",
  //     pTradingAccount->Commission); PURE_LOG("\tCloseProfit [%.8lf]",
  //     pTradingAccount->CloseProfit); PURE_LOG("\tPositionProfit [%.8lf]",
  //     pTradingAccount->PositionProfit); PURE_LOG("\tBalance [%.8lf]",
  //     pTradingAccount->Balance); PURE_LOG("\tAvailable [%.8lf]",
  //     pTradingAccount->Available); PURE_LOG("\tWithdrawQuota [%.8lf]",
  //     pTradingAccount->WithdrawQuota); PURE_LOG("\tReserve [%.8lf]",
  //     pTradingAccount->Reserve); PURE_LOG("\tCredit [%.8lf]",
  //     pTradingAccount->Credit); PURE_LOG("\tMortgage [%.8lf]",
  //     pTradingAccount->Mortgage); PURE_LOG("\tExchangeMargin [%.8lf]",
  //     pTradingAccount->ExchangeMargin); PURE_LOG("\tDeliveryMargin [%.8lf]",
  //     pTradingAccount->DeliveryMargin); PURE_LOG("\tExchangeDeliveryMargin
  //     [%.8lf]", pTradingAccount->ExchangeDeliveryMargin);
  //     PURE_LOG("\tReserveBalance [%.8lf]", pTradingAccount->ReserveBalance);
  //     PURE_LOG("\tPreFundMortgageIn [%.8lf]",
  //     pTradingAccount->PreFundMortgageIn); PURE_LOG("\tPreFundMortgageOut
  //     [%.8lf]", pTradingAccount->PreFundMortgageOut);
  //     PURE_LOG("\tFundMortgageIn [%.8lf]", pTradingAccount->FundMortgageIn);
  //     PURE_LOG("\tFundMortgageOut [%.8lf]",
  //     pTradingAccount->FundMortgageOut); PURE_LOG("\tFundMortgageAvailable
  //     [%.8lf]", pTradingAccount->FundMortgageAvailable);
  //     PURE_LOG("\tMortgageableFund [%.8lf]",
  //     pTradingAccount->MortgageableFund); PURE_LOG("\tSpecProductMargin
  //     [%.8lf]", pTradingAccount->SpecProductMargin);
  //     PURE_LOG("\tSpecProductFrozenMargin [%.8lf]",
  //     pTradingAccount->SpecProductFrozenMargin);
  //     PURE_LOG("\tSpecProductCommission [%.8lf]",
  //     pTradingAccount->SpecProductCommission);
  //     PURE_LOG("\tSpecProductFrozenCommission [%.8lf]",
  //     pTradingAccount->SpecProductFrozenCommission);
  //     PURE_LOG("\tSpecProductPositionProfit [%.8lf]",
  //     pTradingAccount->SpecProductPositionProfit);
  //     PURE_LOG("\tSpecProductCloseProfit [%.8lf]",
  //     pTradingAccount->SpecProductCloseProfit);
  //     PURE_LOG("\tSpecProductPositionProfitByAlg [%.8lf]",
  //     pTradingAccount->SpecProductPositionProfitByAlg);
  //     PURE_LOG("\tSpecProductExchangeMargin [%.8lf]",
  //     pTradingAccount->SpecProductExchangeMargin); PURE_LOG("\tFrozenSwap
  //     [%.8lf]", pTradingAccount->FrozenSwap); PURE_LOG("\tRemainSwap
  //     [%.8lf]", pTradingAccount->RemainSwap);
  // }
  if (pRspInfo) {
    TThostFtdcErrorMsgType msg;
    utils::gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
    PURE_LOG("\tErrorMsg [%s]", msg);
    PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
  }
  // PURE_LOG("\tnRequestID [%d]", nRequestID);
  // PURE_LOG("\tbIsLast [%d]", bIsLast);
  // PURE_LOG("</OnRspQryTradingAccount>");

  if (pTradingAccount) {
    CThostFtdcTradingAccountField *staticTradingAccountField = new CThostFtdcTradingAccountField;
    *staticTradingAccountField = *pTradingAccount;
    MsgStruct msgStruct;
    msgStruct.sessionName = "ctp";
    msgStruct.msgName = "OnRspQryTradingAccount";
    msgStruct.ctpMsg = static_cast<void *>(staticTradingAccountField);
    ctpMsgChan << msgStruct;
  } else {
    MsgStruct msgStruct;
    msgStruct.sessionName = "ctp";
    msgStruct.msgName = "OnRspQryTradingAccount";
    msgStruct.ctpMsg = nullptr;
    ctpMsgChan << msgStruct;
  }
}

void TraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  if (pInstrument) {
    CThostFtdcInstrumentField *staticInstrumentField = new CThostFtdcInstrumentField;
    *staticInstrumentField = *pInstrument;
    MsgStruct msgStruct;
    msgStruct.sessionName = "ctp";
    msgStruct.msgName = "OnRspQryInstrument";
    msgStruct.ctpMsg = static_cast<void *>(staticInstrumentField);
    msgStruct.bIsLast = bIsLast;
    ctpMsgChan << msgStruct;
  } else {
    MsgStruct msgStruct;
    msgStruct.sessionName = "ctp";
    msgStruct.msgName = "OnRspQryInstrument";
    msgStruct.ctpMsg = nullptr;
    msgStruct.bIsLast = bIsLast;
    ctpMsgChan << msgStruct;
  }
}

void TraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                                 bool bIsLast) {
  if (pInputOrderAction == nullptr) {
    ERROR_LOG("pInputOrderAction == nullptr");
    return;
  }
  if (pRspInfo == nullptr) {
    ERROR_LOG("pRspInfo == nullptr");
    return;
  }

  PURE_LOG("<OnRspOrderAction>");
  // if (pInputOrderAction)
  // {
  //     PURE_LOG("\tBrokerID [%s]", pInputOrderAction->BrokerID);
  //     PURE_LOG("\tInvestorID [%s]", pInputOrderAction->InvestorID);
  //     PURE_LOG("\tOrderRef [%s]", pInputOrderAction->OrderRef);
  //     PURE_LOG("\tExchangeID [%s]", pInputOrderAction->ExchangeID);
  //     PURE_LOG("\tOrderSysID [%s]", pInputOrderAction->OrderSysID);
  //     PURE_LOG("\tUserID [%s]", pInputOrderAction->UserID);
  //     PURE_LOG("\tInstrumentID [%s]", pInputOrderAction->InstrumentID);
  //     PURE_LOG("\tInvestUnitID [%s]", pInputOrderAction->InvestUnitID);
  //     PURE_LOG("\tIPAddress [%s]", pInputOrderAction->IPAddress);
  //     PURE_LOG("\tMacAddress [%s]", pInputOrderAction->MacAddress);
  //     PURE_LOG("\tOrderActionRef [%d]", pInputOrderAction->OrderActionRef);
  //     PURE_LOG("\tRequestID [%d]", pInputOrderAction->RequestID);
  //     PURE_LOG("\tFrontID [%d]", pInputOrderAction->FrontID);
  //     PURE_LOG("\tSessionID [%d]", pInputOrderAction->SessionID);
  //     PURE_LOG("\tVolumeChange [%d]", pInputOrderAction->VolumeChange);
  //     PURE_LOG("\tActionFlag [%c]", pInputOrderAction->ActionFlag);
  //     PURE_LOG("\tLimitPrice [%.8lf]", pInputOrderAction->LimitPrice);
  // }
  if (pRspInfo) {
    TThostFtdcErrorMsgType msg;
    utils::gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
    PURE_LOG("\tErrorMsg [%s]", msg);
    PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
  }
  PURE_LOG("\tnRequestID [%d]", nRequestID);
  PURE_LOG("\tbIsLast [%d]", bIsLast);
  PURE_LOG("</OnRspOrderAction>");

  CThostFtdcInputOrderActionField *orderActionField = new CThostFtdcInputOrderActionField;
  *orderActionField = *pInputOrderAction;
  CThostFtdcRspInfoField *msgInfo = new CThostFtdcRspInfoField;
  *msgInfo = *pRspInfo;
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnRspOrderAction";
  msgStruct.ctpMsg = static_cast<void *>(orderActionField);
  msgStruct.ctpMsgInfo = static_cast<void *>(msgInfo);
  msgStruct.bIsLast = bIsLast;
  ctpMsgChan << msgStruct;
}

void TraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) {
  PURE_LOG("<OnErrRtnOrderAction>");
  // if (pOrderAction)
  // {
  //     PURE_LOG("\tBrokerID [%s]", pOrderAction->BrokerID);
  //     PURE_LOG("\tInvestorID [%s]", pOrderAction->InvestorID);
  //     PURE_LOG("\tOrderRef [%s]", pOrderAction->OrderRef);
  //     PURE_LOG("\tExchangeID [%s]", pOrderAction->ExchangeID);
  //     PURE_LOG("\tOrderSysID [%s]", pOrderAction->OrderSysID);
  //     PURE_LOG("\tActionDate [%s]", pOrderAction->ActionDate);
  //     PURE_LOG("\tActionTime [%s]", pOrderAction->ActionTime);
  //     PURE_LOG("\tTraderID [%s]", pOrderAction->TraderID);
  //     PURE_LOG("\tOrderLocalID [%s]", pOrderAction->OrderLocalID);
  //     PURE_LOG("\tActionLocalID [%s]", pOrderAction->ActionLocalID);
  //     PURE_LOG("\tParticipantID [%s]", pOrderAction->ParticipantID);
  //     PURE_LOG("\tClientID [%s]", pOrderAction->ClientID);
  //     PURE_LOG("\tBusinessUnit [%s]", pOrderAction->BusinessUnit);
  //     PURE_LOG("\tUserID [%s]", pOrderAction->UserID);
  //     PURE_LOG("\tStatusMsg [%s]", pOrderAction->StatusMsg);
  //     PURE_LOG("\tInstrumentID [%s]", pOrderAction->InstrumentID);
  //     PURE_LOG("\tBranchID [%s]", pOrderAction->BranchID);
  //     PURE_LOG("\tInvestUnitID [%s]", pOrderAction->InvestUnitID);
  //     PURE_LOG("\tIPAddress [%s]", pOrderAction->IPAddress);
  //     PURE_LOG("\tMacAddress [%s]", pOrderAction->MacAddress);
  //     PURE_LOG("\tOrderActionRef [%d]", pOrderAction->OrderActionRef);
  //     PURE_LOG("\tRequestID [%d]", pOrderAction->RequestID);
  //     PURE_LOG("\tFrontID [%d]", pOrderAction->FrontID);
  //     PURE_LOG("\tSessionID [%d]", pOrderAction->SessionID);
  //     PURE_LOG("\tVolumeChange [%d]", pOrderAction->VolumeChange);
  //     PURE_LOG("\tInstallID [%d]", pOrderAction->InstallID);
  //     PURE_LOG("\tActionFlag [%c]", pOrderAction->ActionFlag);
  //     PURE_LOG("\tOrderActionStatus [%c]", pOrderAction->OrderActionStatus);
  //     PURE_LOG("\tLimitPrice [%.8lf]", pOrderAction->LimitPrice);
  // }
  if (pRspInfo) {
    TThostFtdcErrorMsgType msg;
    utils::gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
    PURE_LOG("\tErrorMsg [%s]", msg);
    PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
  }
  PURE_LOG("</OnErrRtnOrderAction>");

  CThostFtdcOrderActionField *orderActionField = new CThostFtdcOrderActionField;
  *orderActionField = *pOrderAction;
  CThostFtdcRspInfoField *msgInfo = new CThostFtdcRspInfoField;
  *msgInfo = *pRspInfo;
  MsgStruct msgStruct;
  msgStruct.sessionName = "ctp";
  msgStruct.msgName = "OnErrRtnOrderAction";
  msgStruct.ctpMsg = orderActionField;
  msgStruct.ctpMsgInfo = msgInfo;
  ctpMsgChan << msgStruct;
};

void TraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo,
                                             int nRequestID, bool bIsLast) {
  // PURE_LOG("<OnRspQryInstrumentMarginRate>");
  // if (pInstrumentMarginRate)
  // {
  //     PURE_LOG("\tInvestorRange [%d]", pInstrumentMarginRate->InvestorRange);
  //     PURE_LOG("\tBrokerID [%s]", pInstrumentMarginRate->BrokerID);
  //     PURE_LOG("\tInvestorID [%s]", pInstrumentMarginRate->InvestorID);
  //     PURE_LOG("\tHedgeFlag [%d]", pInstrumentMarginRate->HedgeFlag);
  //     PURE_LOG("\tLongMarginRatioByMoney [%.8lf]",
  //     pInstrumentMarginRate->LongMarginRatioByMoney);
  //     PURE_LOG("\tLongMarginRatioByVolume [%.8lf]",
  //     pInstrumentMarginRate->LongMarginRatioByVolume);
  //     PURE_LOG("\tShortMarginRatioByMoney [%.8lf]",
  //     pInstrumentMarginRate->ShortMarginRatioByMoney);
  //     PURE_LOG("\tShortMarginRatioByVolume [%.8lf]",
  //     pInstrumentMarginRate->ShortMarginRatioByVolume);
  //     PURE_LOG("\tIsRelative [%d]", pInstrumentMarginRate->IsRelative);
  //     PURE_LOG("\tExchangeID [%s]", pInstrumentMarginRate->ExchangeID);
  //     PURE_LOG("\tInvestUnitID [%s]", pInstrumentMarginRate->InvestUnitID);
  //     PURE_LOG("\tInstrumentID [%s]", pInstrumentMarginRate->InstrumentID);
  // }
  if (pRspInfo) {
    TThostFtdcErrorMsgType msg;
    utils::gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
    PURE_LOG("\tErrorMsg [%s]", msg);
    PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
  }
  // PURE_LOG("\tnRequestID [%d]", nRequestID);
  // PURE_LOG("\tbIsLast [%d]", bIsLast);
  // PURE_LOG("</OnRspQryInstrumentMarginRate>");

  if (pInstrumentMarginRate) {
    CThostFtdcInstrumentMarginRateField *staticInstrumentMarginRateField = new CThostFtdcInstrumentMarginRateField;
    *staticInstrumentMarginRateField = *pInstrumentMarginRate;
    MsgStruct msgStruct;
    msgStruct.sessionName = "ctp";
    msgStruct.msgName = "OnRspQryInstrumentMarginRate";
    msgStruct.ctpMsg = static_cast<void *>(staticInstrumentMarginRateField);
    ctpMsgChan << msgStruct;
  } else {
    MsgStruct msgStruct;
    msgStruct.sessionName = "ctp";
    msgStruct.msgName = "OnRspQryInstrumentMarginRate";
    msgStruct.ctpMsg = nullptr;
    ctpMsgChan << msgStruct;
  }
};

void TraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate,
                                                 CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  // PURE_LOG("<OnRspQryInstrumentCommissionRate>");
  // if (pInstrumentCommissionRate)
  // {
  //     PURE_LOG("\tInvestorRange [%d]",
  //     pInstrumentCommissionRate->InvestorRange); PURE_LOG("\tBrokerID [%s]",
  //     pInstrumentCommissionRate->BrokerID); PURE_LOG("\tInvestorID [%s]",
  //     pInstrumentCommissionRate->InvestorID); PURE_LOG("\tOpenRatioByMoney
  //     [%.8lf]", pInstrumentCommissionRate->OpenRatioByMoney);
  //     PURE_LOG("\tOpenRatioByVolume [%.8lf]",
  //     pInstrumentCommissionRate->OpenRatioByVolume);
  //     PURE_LOG("\tCloseRatioByMoney [%.8lf]",
  //     pInstrumentCommissionRate->CloseRatioByMoney);
  //     PURE_LOG("\tCloseRatioByVolume [%.8lf]",
  //     pInstrumentCommissionRate->CloseRatioByVolume);
  //     PURE_LOG("\tCloseTodayRatioByMoney [%.8lf]",
  //     pInstrumentCommissionRate->CloseTodayRatioByMoney);
  //     PURE_LOG("\tCloseTodayRatioByVolume [%.8lf]",
  //     pInstrumentCommissionRate->CloseTodayRatioByVolume);
  //     PURE_LOG("\tExchangeID [%s]", pInstrumentCommissionRate->ExchangeID);
  //     PURE_LOG("\tBizType [%d]", pInstrumentCommissionRate->BizType);
  //     PURE_LOG("\tInvestUnitID [%s]",
  //     pInstrumentCommissionRate->InvestUnitID); PURE_LOG("\tInstrumentID
  //     [%s]", pInstrumentCommissionRate->InstrumentID);
  // }
  if (pRspInfo) {
    TThostFtdcErrorMsgType msg;
    utils::gbk2utf8(pRspInfo->ErrorMsg, msg, sizeof(msg));
    PURE_LOG("\tErrorMsg [%s]", msg);
    PURE_LOG("\tErrorID [%d]", pRspInfo->ErrorID);
  }
  // PURE_LOG("\tnRequestID [%d]", nRequestID);
  // PURE_LOG("\tbIsLast [%d]", bIsLast);
  // PURE_LOG("</OnRspQryInstrumentCommissionRate>");

  if (pInstrumentCommissionRate) {
    CThostFtdcInstrumentCommissionRateField *staticInstrumentCommissionRateField = new CThostFtdcInstrumentCommissionRateField;
    *staticInstrumentCommissionRateField = *pInstrumentCommissionRate;
    MsgStruct msgStruct;
    msgStruct.sessionName = "ctp";
    msgStruct.msgName = "OnRspQryInstrumentCommissionRate";
    msgStruct.ctpMsg = static_cast<void *>(staticInstrumentCommissionRateField);
    ctpMsgChan << msgStruct;
  } else {
    MsgStruct msgStruct;
    msgStruct.sessionName = "ctp";
    msgStruct.msgName = "OnRspQryInstrumentCommissionRate";
    msgStruct.ctpMsg = nullptr;
    ctpMsgChan << msgStruct;
  }
};
