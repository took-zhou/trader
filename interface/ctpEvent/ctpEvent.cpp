/*
 * ctpEvent.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/interface/ctpEvent/ctpEvent.h"
#include <algorithm>
#include <mutex>
#include "common/extern/log/log.h"
#include "common/self/semaphorePart.h"
#include "common/self/utils.h"
#include "trader/domain/components/order.h"
#include "trader/domain/traderService.h"
#include "trader/infra/define.h"
#include "trader/interface/marketEvent/marketEvent.h"
#include "trader/interface/strategyEvent/strategyEvent.h"
extern GlobalSem globalSem;
std::mutex m;
bool CtpEvent::init() {
  regMsgFun();

  return true;
}

void CtpEvent::regMsgFun() {
  int cnt = 0;
  msgFuncMap.clear();
  msgFuncMap["OnRspSettlementInfoConfirm"] = [this](MsgStruct &msg) { OnRspSettlementInfoConfirmHandle(msg); };
  msgFuncMap["OnRspUserLogin"] = [this](MsgStruct &msg) { OnRspUserLoginHandle(msg); };
  msgFuncMap["OnRspSettlementInfoConfirm"] = [this](MsgStruct &msg) { OnRspSettlementInfoConfirmHandle(msg); };
  msgFuncMap["OnErrRtnOrderInsert"] = [this](MsgStruct &msg) { OnErrRtnOrderInsertHandle(msg); };
  msgFuncMap["OnRspOrderInsert"] = [this](MsgStruct &msg) { OnRspOrderInsertHandle(msg); };
  msgFuncMap["OnRtnOrder"] = [this](MsgStruct &msg) { OnRtnOrderHandle(msg); };
  msgFuncMap["OnRtnTrade"] = [this](MsgStruct &msg) { OnRtnTradeHandle(msg); };
  msgFuncMap["OnRspQryTradingAccount"] = [this](MsgStruct &msg) { OnRspQryTradingAccountHandle(msg); };
  msgFuncMap["OnRspQryInstrument"] = [this](MsgStruct &msg) { OnRspQryInstrumentHandle(msg); };
  msgFuncMap["OnRspOrderAction"] = [this](MsgStruct &msg) { OnRspOrderActionHandle(msg); };
  msgFuncMap["OnErrRtnOrderAction"] = [this](MsgStruct &msg) { OnErrRtnOrderActionHandle(msg); };
  msgFuncMap["OnRspQryInstrumentMarginRate"] = [this](MsgStruct &msg) { OnRspQryInstrumentMarginRateHandle(msg); };
  msgFuncMap["OnRspQryInstrumentCommissionRate"] = [this](MsgStruct &msg) { OnRspQryInstrumentCommissionRateHandle(msg); };

  for (auto &iter : msgFuncMap) {
    INFO_LOG("msgFuncMap[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void CtpEvent::handle(MsgStruct &msg) {
  auto iter = msgFuncMap.find(msg.msgName);
  if (iter != msgFuncMap.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msgName.c_str());
  return;
}

void CtpEvent::OnErrRtnOrderActionHandle(MsgStruct &msg) {
  CThostFtdcOrderActionField *orderActionRsp = (CThostFtdcOrderActionField *)msg.ctpMsg;
  CThostFtdcRspInfoField *msgInfo = (CThostFtdcRspInfoField *)(msg.ctpMsgInfo);

  std::string orderRef = std::string(orderActionRsp->OrderRef);
  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);
  if (orderManage.orderMaps.find(orderRef) != orderManage.orderMaps.end()) {
    auto &identityId = orderManage.orderMaps.at(orderRef)->identityId;
    strategy_trader::message rspMsg;
    auto *orderCancelRsp = rspMsg.mutable_order_cancel_rsp();
    orderCancelRsp->set_identity(identityId.identity);
    orderCancelRsp->set_result(strategy_trader::Result::failed);
    TThostFtdcErrorMsgType errmsg;
    utils::gbk2utf8(msgInfo->ErrorMsg, errmsg, sizeof(errmsg));
    std::string reason = std::string("OnErrRtnOrderAction_") + utils::intToString(msgInfo->ErrorID) + "_" + std::string(errmsg);
    orderCancelRsp->set_failedreason(reason);
    ROLE(StrategyEvent).pubOrderCancelRsp(identityId, false, reason);
  }
}

void CtpEvent::OnRspOrderActionHandle(MsgStruct &msg) {
  CThostFtdcInputOrderActionField *orderActionRsp = (CThostFtdcInputOrderActionField *)msg.ctpMsg;
  CThostFtdcRspInfoField *msgInfo = (CThostFtdcRspInfoField *)(msg.ctpMsgInfo);

  std::string orderRef = std::string(orderActionRsp->OrderRef);
  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);
  if (orderManage.orderMaps.find(orderRef) != orderManage.orderMaps.end()) {
    auto &identityId = orderManage.orderMaps.at(orderRef)->identityId;
    strategy_trader::message rspMsg;
    auto *orderCancelRsp = rspMsg.mutable_order_cancel_rsp();
    orderCancelRsp->set_identity(identityId.identity);
    orderCancelRsp->set_result(strategy_trader::Result::failed);
    TThostFtdcErrorMsgType errmsg;
    utils::gbk2utf8(msgInfo->ErrorMsg, errmsg, sizeof(errmsg));
    std::string reason = std::string("OnRspOrderAction_") + utils::intToString(msgInfo->ErrorID) + "_" + std::string(errmsg);
    orderCancelRsp->set_failedreason(reason);
    ROLE(StrategyEvent).pubOrderCancelRsp(identityId, false, reason);
  }
}

void CtpEvent::OnRspQryTradingAccountHandle(MsgStruct &msg) {
  CThostFtdcTradingAccountField *tradeAccount = static_cast<CThostFtdcTradingAccountField *>(msg.ctpMsg);
  auto &traderSer = TraderSevice::getInstance();
  auto &tmpAccountInfo = traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo;

  if (tradeAccount) {
    tmpAccountInfo.Available = utils::doubleToStringConvert(tradeAccount->Available);
    tmpAccountInfo.Balance = utils::doubleToStringConvert(tradeAccount->Balance);
    tmpAccountInfo.rsp_is_null = false;
  } else {
    tmpAccountInfo.rsp_is_null = true;
  }

  std::string semName = "trader_ReqQryTradingAccount";
  globalSem.postSemBySemName(semName);
}

void CtpEvent::OnRtnTradeHandle(MsgStruct &msg) {
  CThostFtdcTradeField *pTrade = (CThostFtdcTradeField *)msg.ctpMsg;
  std::string orderRef = std::string(pTrade->OrderRef);

  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);

  if (orderManage.orderMaps.find(orderRef) != orderManage.orderMaps.end()) {
#ifdef BENCH_TEST
    ScopedTimer t("OnRtnTradeHandle");
#endif
    auto &orderContent = orderManage.orderMaps.at(orderRef);
    orderContent->tradedOrder.price = pTrade->Price;
    orderContent->tradedOrder.volume = pTrade->Volume;
    orderContent->tradedOrder.direction = utils::charToString(pTrade->Direction);
    orderContent->tradedOrder.date = pTrade->TradeDate;
    orderContent->tradedOrder.time = pTrade->TradeTime;

    ROLE(StrategyEvent).pubOrderInsertRsp(orderContent->identityId, true, "success");
    orderContent->left_volume -= pTrade->Volume;
    if (orderContent->left_volume == 0) {
      INFO_LOG("the order was finished, ref[%s],identity[%s]", orderRef.c_str(), orderContent->identityId.identity.c_str());
      orderManage.delOrder(orderRef);
    }
  }
}

void CtpEvent::OnRtnOrderHandle(MsgStruct &msg) {
  CThostFtdcOrderField *pOrder = (CThostFtdcOrderField *)msg.ctpMsg;
  std::string orderKey = std::string(pOrder->OrderRef);

  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);
  if (orderManage.orderMaps.find(orderKey) != orderManage.orderMaps.end()) {
    auto &orderContent = orderManage.orderMaps.at(orderKey);
    orderContent->frontId = pOrder->FrontID;
    orderContent->sessionId = pOrder->SessionID;

    if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled) {
#ifdef BENCH_TEST
      ScopedTimer t("OnRtnOrderHandle");
#endif
      if (orderContent->activeCancleIndication) {
        auto identityId = orderContent->identityId;
        strategy_trader::message rspMsg;
        auto *orderCancelRsp = rspMsg.mutable_order_cancel_rsp();
        orderCancelRsp->set_identity(identityId.identity);
        orderCancelRsp->set_result(strategy_trader::Result::failed);
        TThostFtdcErrorMsgType errmsg;
        std::string reason = std::string("INVALID");
        orderCancelRsp->set_failedreason(reason);
        ROLE(StrategyEvent).pubOrderCancelRsp(identityId, true, reason);
      }
      std::string reason = ORDER_CANCEL;
      ROLE(StrategyEvent).pubOrderInsertRsp(orderContent->identityId, false, reason);
      INFO_LOG("the order be canceled, ref[%s],identity[%s]", orderKey.c_str(), orderContent->identityId.identity.c_str());
      orderManage.delOrder(orderKey);
    }
  }
}

void CtpEvent::OnRspOrderInsertHandle(MsgStruct &msg) {
  CThostFtdcInputOrderField *ctpRspField = static_cast<CThostFtdcInputOrderField *>(msg.ctpMsg);
  CThostFtdcRspInfoField *ctpMsgInfo = static_cast<CThostFtdcRspInfoField *>(msg.ctpMsgInfo);

  // 打印错误信息
  if (ctpMsgInfo) {
    TThostFtdcErrorMsgType msg;
    utils::gbk2utf8(ctpMsgInfo->ErrorMsg, msg, sizeof(msg));

    ERROR_LOG("\tErrorMsg [%s]", msg);
    ERROR_LOG("\tErrorID [%d]", ctpMsgInfo->ErrorID);
  }

  std::string orderKey = std::string(ctpRspField->OrderRef);

  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);
  if (orderManage.orderMaps.find(orderKey) != orderManage.orderMaps.end()) {
    auto &orderContent = orderManage.orderMaps.at(orderKey);

    if (orderContent->isFlowFinish) {
      INFO_LOG("insert flow has finished, ref[%s], identity[%s]", orderKey.c_str(), orderContent->identityId.identity.c_str());
    } else {
      orderContent->isFlowFinish = true;
      std::string reason = ORDER_FILL_ERROR;
      ROLE(StrategyEvent).pubOrderInsertRsp(orderContent->identityId, false, reason);
      orderManage.delOrder(orderKey);
    }
  }
}

void CtpEvent::OnRspUserLoginHandle(MsgStruct &msg) {
  CThostFtdcRspUserLoginField *pMsg = static_cast<CThostFtdcRspUserLoginField *>(msg.ctpMsg);

  auto &traderSer = TraderSevice::getInstance();
  auto &logingInfo = traderSer.ROLE(Trader).ROLE(TmpStore).logingInfo;
  logingInfo.FrontID = pMsg->FrontID;
  logingInfo.SessionID = pMsg->SessionID;
  logingInfo.OrderRef = (unsigned long int)((unsigned int)logingInfo.SessionID % 1000000) * 1000000;

  std::string semName = "trader_logIn";
  globalSem.postSemBySemName(semName);
  INFO_LOG("post sem of [%s]", semName.c_str());
}

void CtpEvent::OnRspSettlementInfoConfirmHandle(MsgStruct &msg) {
  CThostFtdcSettlementInfoConfirmField *pMsg = static_cast<CThostFtdcSettlementInfoConfirmField *>(msg.ctpMsg);
  std::string semName = "trader_ReqSettlementInfoConfirm";
  globalSem.postSemBySemName(semName);
  INFO_LOG("post sem of [%s]", semName.c_str());
}

void CtpEvent::OnErrRtnOrderInsertHandle(MsgStruct &msg) {
  CThostFtdcInputOrderField *ctpRspField = (CThostFtdcInputOrderField *)msg.ctpMsg;
  CThostFtdcRspInfoField *rspInfoField = (CThostFtdcRspInfoField *)msg.ctpMsgInfo;

  std::string orderKey = std::string(ctpRspField->OrderRef);
  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);
  if (orderManage.orderMaps.find(orderKey) != orderManage.orderMaps.end()) {
    auto &orderContent = orderManage.orderMaps.at(orderKey);

    if (orderContent->isFlowFinish) {
      INFO_LOG("insert flow has finished, ref[%s], identity[%s]", orderKey.c_str(), orderContent->identityId.identity.c_str());
    } else {
      orderContent->isFlowFinish = true;
      std::string reason = ORDER_FILL_ERROR;
      ROLE(StrategyEvent).pubOrderInsertRsp(orderContent->identityId, false, reason);
      orderManage.delOrder(orderKey);
    }
  }
}

void CtpEvent::OnRspQryInstrumentHandle(MsgStruct &msg) {
  CThostFtdcInstrumentField *instrumentField = static_cast<CThostFtdcInstrumentField *>(msg.ctpMsg);

  if (globalSem.existSem("trader_ReqQryInstrument_ALL")) {
    if (instrumentField) {
      ROLE(MarketEvent).pubQryInstrumentRsp(instrumentField, true, msg.bIsLast);

      if (msg.bIsLast == true) {
        globalSem.delOrderSem("trader_ReqQryInstrument_ALL");
      }
    }
  } else if (globalSem.existSem("trader_ReqQryInstrument_Single")) {
    auto &traderSer = TraderSevice::getInstance();
    auto &instrumentInfo = traderSer.ROLE(Trader).ROLE(TmpStore).instrumentInfo;

    if (instrumentField) {
      instrumentInfo.is_trading = instrumentField->IsTrading;
      instrumentInfo.max_limit_order_volume = instrumentField->MaxLimitOrderVolume;
      instrumentInfo.max_market_order_volume = instrumentField->MaxMarketOrderVolume;
      instrumentInfo.min_limit_order_volume = instrumentField->MinLimitOrderVolume;
      instrumentInfo.min_market_order_volume = instrumentField->MinMarketOrderVolume;
      instrumentInfo.price_tick = utils::doubleToStringConvert(instrumentField->PriceTick);
      instrumentInfo.volume_multiple = utils::doubleToStringConvert(instrumentField->VolumeMultiple);
      instrumentInfo.rsp_is_null = false;
    } else {
      instrumentInfo.rsp_is_null = true;
    }

    std::string semName = "trader_ReqQryInstrument_Single";
    globalSem.postSemBySemName(semName);
  }
}

void CtpEvent::OnRspQryInstrumentMarginRateHandle(MsgStruct &msg) {
  CThostFtdcInstrumentMarginRateField *marginRateField = static_cast<CThostFtdcInstrumentMarginRateField *>(msg.ctpMsg);
  auto &traderSer = TraderSevice::getInstance();
  auto &marginRate = traderSer.ROLE(Trader).ROLE(TmpStore).marginRate;

  if (marginRateField) {
    marginRate.LongMarginRatioByMoney = utils::doubleToStringConvert(marginRateField->LongMarginRatioByMoney);
    marginRate.LongMarginRatioByVolume = utils::doubleToStringConvert(marginRateField->LongMarginRatioByVolume);
    marginRate.ShortMarginRatioByMoney = utils::doubleToStringConvert(marginRateField->ShortMarginRatioByMoney);
    marginRate.ShortMarginRatioByVolume = utils::doubleToStringConvert(marginRateField->ShortMarginRatioByVolume);
    marginRate.rsp_is_null = false;
  } else {
    marginRate.rsp_is_null = true;
  }

  std::string semName = "margin_rate";
  globalSem.postSemBySemName(semName);
}

void CtpEvent::OnRspQryInstrumentCommissionRateHandle(MsgStruct &msg) {
  CThostFtdcInstrumentCommissionRateField *commissionRateField = static_cast<CThostFtdcInstrumentCommissionRateField *>(msg.ctpMsg);
  auto &traderSer = TraderSevice::getInstance();
  auto &commissionRate = traderSer.ROLE(Trader).ROLE(TmpStore).commissionRate;

  if (commissionRateField) {
    commissionRate.CloseRatioByMoney = utils::doubleToStringConvert(commissionRateField->CloseRatioByMoney);
    commissionRate.CloseRatioByVolume = utils::doubleToStringConvert(commissionRateField->CloseRatioByVolume);
    commissionRate.CloseTodayRatioByMoney = utils::doubleToStringConvert(commissionRateField->CloseTodayRatioByMoney);
    commissionRate.CloseTodayRatioByVolume = utils::doubleToStringConvert(commissionRateField->CloseTodayRatioByVolume);
    commissionRate.OpenRatioByMoney = utils::doubleToStringConvert(commissionRateField->OpenRatioByMoney);
    commissionRate.OpenRatioByVolume = utils::doubleToStringConvert(commissionRateField->OpenRatioByVolume);
    commissionRate.rsp_is_null = false;

  } else {
    commissionRate.rsp_is_null = true;
  }

  std::string semName = "commission_rate";
  globalSem.postSemBySemName(semName);
}
