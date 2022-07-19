/*
 * strategyEvent.cpp
 */
#include <fstream>
#include <iomanip>
#include <iostream>

#include "common/extern/google/protobuf/text_format.h"
#include "common/extern/log/log.h"
#include "common/self/fileUtil.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/utils.h"
#include "trader/domain/traderService.h"
#include "trader/infra/define.h"
#include "trader/infra/recerSender.h"
#include "trader/interface/strategyEvent/strategyEvent.h"

#include "common/self/semaphorePart.h"
extern GlobalSem globalSem;

bool StrategyEvent::init() {
  regMsgFun();
  return true;
}

void StrategyEvent::handle(MsgStruct &msg) {
  auto iter = msgFuncMap.find(msg.msgName);
  if (iter != msgFuncMap.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msgName.c_str());
  return;
}

void StrategyEvent::regMsgFun() {
  int cnt = 0;
  msgFuncMap.clear();
  msgFuncMap["OrderInsertReq"] = [this](MsgStruct &msg) { OrderInsertReqHandle(msg); };
  msgFuncMap["AccountStatusReq"] = [this](MsgStruct &msg) { AccountStatusReqHandle(msg); };
  msgFuncMap["OrderCancelReq"] = [this](MsgStruct &msg) { OrderCancelReqHandle(msg); };
  msgFuncMap["MarginRateReq"] = [this](MsgStruct &msg) { MarginRateReqHandle(msg); };
  msgFuncMap["CommissionRateReq"] = [this](MsgStruct &msg) { CommissionRateReqHandle(msg); };
  msgFuncMap["InstrumentReq"] = [this](MsgStruct &msg) { InstrumentReqHandle(msg); };

  for (auto &iter : msgFuncMap) {
    INFO_LOG("sessionFuncMap[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
  return;
}

void StrategyEvent::OrderCancelReqHandle(MsgStruct &msg) {
  strategy_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);

  auto &orderCancelReq = reqMsg.order_cancel_req();
  OrderIdentify IdentifyId;
  IdentifyId.prid = orderCancelReq.process_random_id();
  IdentifyId.identity = orderCancelReq.identity();
  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.ROLE(Trader).ROLE(CtpTraderApi).getTraderLoginState() != LOGIN_STATE) {
    ERROR_LOG("ctp not login!");
    pubOrderCancelRsp(IdentifyId, false, "ctp_logout");
    return;
  }
  auto &orderManage = traderSer.ROLE(OrderManage);
  for (auto &item : orderManage.orderMaps) {
    if (item.second->identityId.identity == IdentifyId.identity) {
      item.second->activeCancleIndication = true;
      auto *traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;
      traderApi->ReqOrderAction(item.second);
      break;
    }
  }
}

void StrategyEvent::pubOrderCancelRsp(OrderIdentify identityId, bool result, const std::string &reason) {
  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);
  for (auto &item : orderManage.orderMaps) {
    if (item.second->identityId.identity == identityId.identity) {
      if (item.second->isFlowFinish) {
        INFO_LOG("flow has finished identityId[%s]", identityId.identity.c_str());
      } else {
        item.second->isFlowFinish = true;
        strategy_trader::message rspMsg;
        auto *orderCancelRsp = rspMsg.mutable_order_cancel_rsp();
        orderCancelRsp->set_identity(identityId.identity);
        orderCancelRsp->set_result(result ? strategy_trader::Result::success : strategy_trader::Result::failed);
        orderCancelRsp->set_failedreason(reason);

        std::string strRsp = rspMsg.SerializeAsString();
        std::string head = "strategy_trader.OrderCancelRsp";
        auto &recerSender = RecerSender::getInstance();
        bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
      }
      break;
    }
  }
}

void StrategyEvent::AccountStatusReqHandle(MsgStruct &msg) {
  strategy_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);
  auto reqInfo = reqMsg.account_status_req();
  auto identify = reqInfo.process_random_id();
  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.ROLE(Trader).ROLE(CtpTraderApi).getTraderLoginState() != LOGIN_STATE) {
    ERROR_LOG("ctp not login!");
    pubAccountStatusRsp(identify, false, "ctp_logout");
    return;
  }

  traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo.ProcessRandomId = identify;
  auto *traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;

  std::string semName = "trader_ReqQryTradingAccount";
  if (traderApi->ReqQryTradingAccount() != 0) {
    ERROR_LOG("req error!");
    pubAccountStatusRsp(identify, false, "req error");
    globalSem.delOrderSem(semName);
    return;
  }

  if (globalSem.waitSemBySemName(semName, 10) != 0) {
    ERROR_LOG("req timeout!");
    pubAccountStatusRsp(identify, false, "req timeout");
    globalSem.delOrderSem(semName);
    return;
  }
  globalSem.delOrderSem(semName);

  if (traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo.rsp_is_null == true) {
    pubAccountStatusRsp(identify, false, "rsp is null");
    return;
  }
  pubAccountStatusRsp(identify, true);
}

void StrategyEvent::pubAccountStatusRsp(const std::string &identity, bool result, const std::string &reason) {
  strategy_trader::message rsp;
  auto *accountRsp = rsp.mutable_account_status_rsp();
  auto &traderSer = TraderSevice::getInstance();
  accountRsp->set_result(result ? strategy_trader::Result::success : strategy_trader::Result::failed);

  if (result == true) {
    auto &tmpAccountInfo = traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo;

    accountRsp->set_balance(tmpAccountInfo.Balance);
    accountRsp->set_available(tmpAccountInfo.Available);
  } else {
    accountRsp->set_failedreason(reason);
  }

  std::string strRsp = rsp.SerializeAsString();
  std::string head = "strategy_trader.AccountStatusRsp." + identity;
  auto &recerSender = RecerSender::getInstance();
  bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
  if (!sendRes) {
    ERROR_LOG("send OrderInsertRsp error");
    return;
  }
  return;
}

void StrategyEvent::OrderInsertReqHandle(MsgStruct &msg) {
#ifdef BENCH_TEST
  ScopedTimer t("OrderInsertReqHandle");
#endif
  strategy_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);

  const auto &orderInsertReq = reqMsg.order_insert_req();
  OrderIdentify identity;
  identity.identity = orderInsertReq.identity();
  identity.prid = orderInsertReq.process_random_id();
  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.ROLE(Trader).ROLE(CtpTraderApi).getTraderLoginState() != LOGIN_STATE) {
    ERROR_LOG("ctp not login!");
    pubOrderInsertRsp(identity, false, ORDER_BUILD_ERROR);
    return;
  }

  traderSer.ROLE(Trader).ROLE(TmpStore).logingInfo.OrderRef++;
  std::string newOrderRef = utils::unsignedlongintToString(traderSer.ROLE(Trader).ROLE(TmpStore).logingInfo.OrderRef);

  auto &orderManage = traderSer.ROLE(OrderManage);
  if (!orderManage.addOrder(newOrderRef)) {
    ERROR_LOG("add order [%s] to OrderManage error", newOrderRef.c_str());
    pubOrderInsertRsp(identity, false, ORDER_BUILD_ERROR);
    return;
  }

  CThostFtdcInputOrderField newOrder;
  if (!orderManage.buildOrder(newOrderRef, orderInsertReq, newOrder)) {
    ERROR_LOG("build order failed, the orderRef is [%s]", newOrderRef.c_str());
    pubOrderInsertRsp(identity, false, ORDER_BUILD_ERROR);
    orderManage.delOrder(newOrderRef);
    return;
  }

  auto &orderContent = orderManage.orderMaps.at(newOrderRef);
  orderContent->orderRef = newOrderRef;
  orderContent->identityId.prid = identity.prid;
  orderContent->identityId.identity = identity.identity;
  orderContent->instrumentID = newOrder.InstrumentID;
  orderContent->investorId = newOrder.InvestorID;
  orderContent->userId = newOrder.UserID;
  orderContent->total_volume = newOrder.VolumeTotalOriginal;
  orderContent->left_volume = newOrder.VolumeTotalOriginal;
  orderContent->limit_price = newOrder.LimitPrice;

  auto *traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;
  traderApi->ReqOrderInsert(&newOrder);
  return;
}

void StrategyEvent::pubOrderInsertRsp(OrderIdentify identity, bool result, std::string reason) {
  strategy_trader::message rsp;
  auto *insertRsp = rsp.mutable_order_insert_rsp();
  insertRsp->set_identity(identity.identity);
  auto rspResult = result ? strategy_trader::Result::success : strategy_trader::Result::failed;
  insertRsp->set_result(rspResult);
  if (result) {
    auto *succInfo = insertRsp->mutable_info();
    if (succInfo == nullptr) {
      ERROR_LOG("mutable_info error! identity[%s]", identity.identity.c_str());
      return;
    }
    auto &traderSer = TraderSevice::getInstance();
    auto &orderManage = traderSer.ROLE(OrderManage);

    for (auto &item : orderManage.orderMaps) {
      if (item.second->identityId.identity == identity.identity) {
        succInfo->set_orderprice(utils::doubleToStringConvert(item.second->tradedOrder.price));
        succInfo->set_ordervolume(item.second->tradedOrder.volume);
      }
      break;
    }
  }

  if (!result) {
    if (reason == std::string(ORDER_BUILD_ERROR)) {
      insertRsp->set_reason(strategy_trader::FailedReason::Strategy_Ind_Error);
    }
    if (reason == std::string(ORDER_FILL_ERROR)) {
      insertRsp->set_reason(strategy_trader::FailedReason::Order_Fill_Error);
    }
    if (reason == std::string(ORDER_CANCEL)) {
      insertRsp->set_reason(strategy_trader::FailedReason::Order_Cancel);
    }
  }

  std::string strRsp = rsp.SerializeAsString();
  std::string head = "strategy_trader.OrderInsertRsp." + identity.prid;
  auto &recerSender = RecerSender::getInstance();
  bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
  if (!sendRes) {
    ERROR_LOG("send OrderInsertRsp error");
  }

  if (result) {
    sendEmail(identity.identity);
  }

  return;
}

void StrategyEvent::MarginRateReqHandle(MsgStruct &msg) {
  strategy_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);
  auto identify = reqMsg.margin_rate_req().process_random_id();
  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.ROLE(Trader).ROLE(CtpTraderApi).getTraderLoginState() != LOGIN_STATE) {
    ERROR_LOG("ctp not login!");
    pubMarginRateRsp(identify, false, "ctp_logout");
    return;
  }

  traderSer.ROLE(Trader).ROLE(TmpStore).marginRate.ProcessRandomId = identify;
  utils::InstrumtntID ins_exch;
  ins_exch.ins = reqMsg.margin_rate_req().instrument_info().instrument_id();
  ins_exch.exch = reqMsg.margin_rate_req().instrument_info().exchange_id();
  auto *traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;

  std::string semName = "margin_rate";
  if (traderApi->ReqQryInstrumentMarginRate(ins_exch) != 0) {
    ERROR_LOG("req error!");
    pubMarginRateRsp(identify, false, "req error");
    globalSem.delOrderSem(semName);
    return;
  }

  if (globalSem.waitSemBySemName(semName, 10) != 0) {
    ERROR_LOG("req timeout!");
    pubMarginRateRsp(identify, false, "req timeout");
    globalSem.delOrderSem(semName);
    return;
  }
  globalSem.delOrderSem(semName);

  if (traderSer.ROLE(Trader).ROLE(TmpStore).marginRate.rsp_is_null == true) {
    pubMarginRateRsp(identify, false, "rsp is null");
    return;
  }

  pubMarginRateRsp(identify, true);
}

void StrategyEvent::pubMarginRateRsp(const std::string &identity, bool result, const std::string &reason) {
  strategy_trader::message rsp;
  auto *marginRateRsp = rsp.mutable_margin_rate_rsp();
  auto &traderSer = TraderSevice::getInstance();
  marginRateRsp->set_result(result ? strategy_trader::Result::success : strategy_trader::Result::failed);

  if (result == true) {
    auto &marginRateInfo = traderSer.ROLE(Trader).ROLE(TmpStore).marginRate;
    marginRateRsp->set_longmarginratiobymoney(marginRateInfo.LongMarginRatioByMoney);
    marginRateRsp->set_longmarginratiobyvolume(marginRateInfo.LongMarginRatioByVolume);
    marginRateRsp->set_shortmarginratiobymoney(marginRateInfo.ShortMarginRatioByMoney);
    marginRateRsp->set_shortmarginratiobyvolume(marginRateInfo.ShortMarginRatioByVolume);
  } else {
    marginRateRsp->set_failedreason(reason);
  }

  std::string strRsp = rsp.SerializeAsString();
  std::string head = "strategy_trader.MarginRateRsp." + identity;
  auto &recerSender = RecerSender::getInstance();
  bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
  if (!sendRes) {
    ERROR_LOG("send OrderInsertRsp error");
    return;
  }
  return;
}

void StrategyEvent::CommissionRateReqHandle(MsgStruct &msg) {
  strategy_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);
  auto identify = reqMsg.commission_rate_req().process_random_id();
  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.ROLE(Trader).ROLE(CtpTraderApi).getTraderLoginState() != LOGIN_STATE) {
    ERROR_LOG("ctp not login!");
    pubCommissionRateRsp(identify, false, "ctp_logout");
    return;
  }

  traderSer.ROLE(Trader).ROLE(TmpStore).commissionRate.ProcessRandomId = identify;
  utils::InstrumtntID ins_exch;
  ins_exch.ins = reqMsg.commission_rate_req().instrument_info().instrument_id();
  ins_exch.exch = reqMsg.commission_rate_req().instrument_info().exchange_id();
  auto *traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;

  std::string semName = "commission_rate";
  if (traderApi->ReqQryInstrumentCommissionRate(ins_exch) != 0) {
    ERROR_LOG("req error!");
    pubCommissionRateRsp(identify, false, "req error");
    globalSem.delOrderSem(semName);
    return;
  }

  if (globalSem.waitSemBySemName(semName, 10) != 0) {
    ERROR_LOG("req timeout!");
    pubCommissionRateRsp(identify, false, "req timeout");
    globalSem.delOrderSem(semName);
    return;
  }
  globalSem.delOrderSem(semName);

  if (traderSer.ROLE(Trader).ROLE(TmpStore).commissionRate.rsp_is_null == true) {
    pubCommissionRateRsp(identify, false, "rsp is null");
    return;
  }

  pubCommissionRateRsp(identify, true);
}

void StrategyEvent::pubCommissionRateRsp(const std::string &identity, bool result, const std::string &reason) {
  strategy_trader::message rsp;
  auto *commissionRateRsp = rsp.mutable_commission_rate_rsp();
  auto &traderSer = TraderSevice::getInstance();
  commissionRateRsp->set_result(result ? strategy_trader::Result::success : strategy_trader::Result::failed);

  if (result == true) {
    auto &commissionRateInfo = traderSer.ROLE(Trader).ROLE(TmpStore).commissionRate;
    commissionRateRsp->set_openratiobymoney(commissionRateInfo.OpenRatioByMoney);
    commissionRateRsp->set_openratiobyvolume(commissionRateInfo.OpenRatioByVolume);
    commissionRateRsp->set_closeratiobymoney(commissionRateInfo.CloseRatioByMoney);
    commissionRateRsp->set_closeratiobyvolume(commissionRateInfo.CloseRatioByVolume);
    commissionRateRsp->set_closetodayratiobymoney(commissionRateInfo.CloseTodayRatioByMoney);
    commissionRateRsp->set_closetodayratiobyvolume(commissionRateInfo.CloseTodayRatioByVolume);
  } else {
    commissionRateRsp->set_failedreason(reason);
  }

  std::string strRsp = rsp.SerializeAsString();
  std::string head = "strategy_trader.CommissionRateRsp." + identity;
  auto &recerSender = RecerSender::getInstance();
  bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
  if (!sendRes) {
    ERROR_LOG("send CommissionRateRsp error");
    return;
  }
  return;
}

void StrategyEvent::InstrumentReqHandle(MsgStruct &msg) {
  strategy_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);
  auto identify = reqMsg.instrument_req().process_random_id();
  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.ROLE(Trader).ROLE(CtpTraderApi).getTraderLoginState() != LOGIN_STATE) {
    ERROR_LOG("ctp not login!");
    pubInstrumentRsp(identify, false, "ctp_logout");
    return;
  }

  traderSer.ROLE(Trader).ROLE(TmpStore).instrumentInfo.ProcessRandomId = identify;
  utils::InstrumtntID ins_exch;
  ins_exch.ins = reqMsg.instrument_req().instrument_info().instrument_id();
  ins_exch.exch = reqMsg.instrument_req().instrument_info().exchange_id();
  auto *traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;

  std::string semName = "trader_ReqQryInstrument_Single";
  if (traderApi->ReqQryInstrument(ins_exch) != 0) {
    ERROR_LOG("req error!");
    pubInstrumentRsp(identify, false, "req error");
    globalSem.delOrderSem(semName);
    return;
  }

  if (globalSem.waitSemBySemName(semName, 10) != 0) {
    ERROR_LOG("req timeout!");
    pubInstrumentRsp(identify, false, "req timeout");
    globalSem.delOrderSem(semName);
    return;
  }
  globalSem.delOrderSem(semName);

  if (traderSer.ROLE(Trader).ROLE(TmpStore).instrumentInfo.rsp_is_null == true) {
    pubInstrumentRsp(identify, false, "rsp is null");
    return;
  }

  pubInstrumentRsp(identify, true);
}

void StrategyEvent::pubInstrumentRsp(const std::string &identity, bool result, const std::string &reason) {
  strategy_trader::message rsp;
  auto *instrumentRsp = rsp.mutable_instrument_rsp();
  auto &traderSer = TraderSevice::getInstance();
  instrumentRsp->set_result(result ? strategy_trader::Result::success : strategy_trader::Result::failed);

  if (result == true) {
    auto &instrumentInfo = traderSer.ROLE(Trader).ROLE(TmpStore).instrumentInfo;
    instrumentRsp->set_is_trading(instrumentInfo.is_trading);
    instrumentRsp->set_max_limit_order_volume(instrumentInfo.max_limit_order_volume);
    instrumentRsp->set_max_market_order_volume(instrumentInfo.max_market_order_volume);
    instrumentRsp->set_min_limit_order_volume(instrumentInfo.min_limit_order_volume);
    instrumentRsp->set_min_market_order_volume(instrumentInfo.min_market_order_volume);
    instrumentRsp->set_price_tick(instrumentInfo.price_tick);
    instrumentRsp->set_volume_multiple(instrumentInfo.volume_multiple);
  } else {
    instrumentRsp->set_failedreason(reason);
  }

  std::string strRsp = rsp.SerializeAsString();
  std::string head = "strategy_trader.InstrumentRsp." + identity;
  auto &recerSender = RecerSender::getInstance();
  bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
  if (!sendRes) {
    ERROR_LOG("send InstrumentRsp error");
    return;
  }
  return;
}

bool StrategyEvent::sendEmail(const std::string &identity) {
  const auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);

  for (auto &item : orderManage.orderMaps) {
    if (item.second->identityId.identity == identity) {
      std::string subjectContent = "";
      subjectContent += item.second->instrumentID + "成交通知";

      std::string saveContent = "";
      saveContent += "账户: " + item.second->userId + "\n";
      saveContent += "合约: " + item.second->instrumentID + "\n";
      saveContent += "下单价格: " + utils::doubleToStringConvert(item.second->limit_price) + "\n";
      saveContent += "成交价格: " + utils::doubleToStringConvert(item.second->tradedOrder.price) + "\n";
      saveContent += "成交日期: " + item.second->tradedOrder.date + "\n";
      saveContent += "成交时间: " + item.second->tradedOrder.time + "\n";
      std::string direction = (item.second->tradedOrder.direction == "0") ? "BUY" : "SELL";
      saveContent += "方向: " + direction + "\n";
      saveContent += "下单数量: " + utils::intToString(item.second->total_volume) + "\n";
      saveContent += "本批成交数量: " + utils::intToString(item.second->tradedOrder.volume) + "\n";

      auto &recerSender = RecerSender::getInstance();
      recerSender.ROLE(Sender).ROLE(EmailSender).send(subjectContent.c_str(), saveContent.c_str());
      break;
    }
  }
  return true;
}
