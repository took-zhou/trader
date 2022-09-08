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
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/protobuf/manage-trader.pb.h"
#include "common/self/protobuf/market-trader.pb.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphorePart.h"
#include "common/self/utils.h"
#include "trader/domain/traderService.h"
#include "trader/infra/recerSender.h"

CtpEvent::CtpEvent() { regMsgFun(); }

void CtpEvent::regMsgFun() {
  int cnt = 0;
  msgFuncMap.clear();
  msgFuncMap["OnRtnOrder"] = [this](utils::ItpMsg &msg) { OnRtnOrderHandle(msg); };
  msgFuncMap["OnRtnTrade"] = [this](utils::ItpMsg &msg) { OnRtnTradeHandle(msg); };
  msgFuncMap["OnRspOrderAction"] = [this](utils::ItpMsg &msg) { OnRspOrderActionHandle(msg); };
  msgFuncMap["OnRspQryInstrument"] = [this](utils::ItpMsg &msg) { OnRspQryInstrumentHandle(msg); };
  msgFuncMap["OnRspQryTradingAccount"] = [this](utils::ItpMsg &msg) { OnRspQryTradingAccountHandle(msg); };
  msgFuncMap["OnRspQryInstrumentMarginRate"] = [this](utils::ItpMsg &msg) { OnRspQryInstrumentMarginRateHandle(msg); };
  msgFuncMap["OnRspQryInstrumentCommissionRate"] = [this](utils::ItpMsg &msg) { OnRspQryInstrumentCommissionRateHandle(msg); };

  for (auto &iter : msgFuncMap) {
    INFO_LOG("msgFuncMap[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void CtpEvent::handle(utils::ItpMsg &msg) {
  auto iter = msgFuncMap.find(msg.msgName);
  if (iter != msgFuncMap.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msgName.c_str());
  return;
}

void CtpEvent::OnRtnOrderHandle(utils::ItpMsg &msg) {
  ipc::message itpMsg;
  itpMsg.ParseFromString(msg.pbMsg);
  auto &itp_msg = itpMsg.itp_msg();

  auto pOrder = reinterpret_cast<CThostFtdcOrderField *>(itp_msg.address());
  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);
  auto content = orderManage.getOrder(pOrder->OrderRef);
  if (content != nullptr) {
    if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled) {
#ifdef BENCH_TEST
      ScopedTimer t("OnRtnOrderHandle");
#endif
      strategy_trader::message rspMsg;
      auto *orderCancelRsp = rspMsg.mutable_order_cancel_rsp();
      orderCancelRsp->set_identity(pOrder->OrderRef);
      orderCancelRsp->set_result(strategy_trader::Result::success);

      utils::ItpMsg msg;
      rspMsg.SerializeToString(&msg.pbMsg);
      msg.sessionName = "strategy_trader";
      msg.msgName = "OrderCancelRsp." + content->prid;
      auto &recerSender = RecerSender::getInstance();
      recerSender.ROLE(Sender).ROLE(ProxySender).send(msg);

      strategy_trader::message rsp;
      auto *insertRsp = rsp.mutable_order_insert_rsp();
      insertRsp->set_identity(content->orderRef);
      insertRsp->set_result(strategy_trader::Result::failed);
      insertRsp->set_reason(strategy_trader::FailedReason::Order_Cancel);

      rsp.SerializeToString(&msg.pbMsg);
      msg.sessionName = "strategy_trader";
      msg.msgName = "OrderInsertRsp." + content->prid;
      recerSender.ROLE(Sender).ROLE(ProxySender).send(msg);

      INFO_LOG("the order be canceled, order ref: %s, prid: %s.", pOrder->OrderRef, content->prid.c_str());
      orderManage.delOrder(pOrder->OrderRef);
    }
  }
}

void CtpEvent::OnRtnTradeHandle(utils::ItpMsg &msg) {
  ipc::message itpMsg;
  itpMsg.ParseFromString(msg.pbMsg);
  auto &itp_msg = itpMsg.itp_msg();

  auto pTrade = reinterpret_cast<CThostFtdcTradeField *>(itp_msg.address());
  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);

#ifdef BENCH_TEST
  ScopedTimer t("OnRtnTradeHandle");
#endif
  auto content = orderManage.getOrder(pTrade->OrderRef);
  if (content != nullptr) {
    content->tradedOrder.price = pTrade->Price;
    content->tradedOrder.volume = pTrade->Volume;
    content->tradedOrder.direction = std::to_string(pTrade->Direction);
    content->tradedOrder.date = pTrade->TradeDate;
    content->tradedOrder.time = pTrade->TradeTime;

    strategy_trader::message rsp;
    auto *insertRsp = rsp.mutable_order_insert_rsp();
    insertRsp->set_identity(pTrade->OrderRef);
    insertRsp->set_result(strategy_trader::Result::success);

    auto *succInfo = insertRsp->mutable_info();
    succInfo->set_orderprice(content->tradedOrder.price);
    succInfo->set_ordervolume(content->tradedOrder.volume);

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pbMsg);
    msg.sessionName = "strategy_trader";
    msg.msgName = "OrderInsertRsp." + content->prid;
    auto &recerSender = RecerSender::getInstance();
    recerSender.ROLE(Sender).ROLE(ProxySender).send(msg);

    sendEmail(*content);

    content->left_volume -= pTrade->Volume;
    if (content->left_volume == 0) {
      INFO_LOG("the order was finished, order ref: %s, prid: %s.", pTrade->OrderRef, content->prid.c_str());
      orderManage.delOrder(pTrade->OrderRef);
    }
  }
}

void CtpEvent::OnRspOrderActionHandle(utils::ItpMsg &msg) {
  ipc::message itpMsg;
  itpMsg.ParseFromString(msg.pbMsg);
  auto &itp_msg = itpMsg.itp_msg();

  auto orderActionRsp = reinterpret_cast<CThostFtdcInputOrderActionField *>(itp_msg.address());
  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);
  auto content = orderManage.getOrder(orderActionRsp->OrderRef);
  if (content != nullptr) {
    strategy_trader::message rspMsg;
    auto *orderCancelRsp = rspMsg.mutable_order_cancel_rsp();
    orderCancelRsp->set_identity(orderActionRsp->OrderRef);
    orderCancelRsp->set_result(strategy_trader::Result::failed);
    orderCancelRsp->set_failedreason("INVALID");

    utils::ItpMsg msg;
    rspMsg.SerializeToString(&msg.pbMsg);
    msg.sessionName = "strategy_trader";
    msg.msgName = "OrderCancelRsp." + content->prid;
    auto &recerSender = RecerSender::getInstance();
    recerSender.ROLE(Sender).ROLE(ProxySender).send(msg);
  }
}

void CtpEvent::OnRspQryTradingAccountHandle(utils::ItpMsg &msg) {
  ipc::message itpMsg;
  itpMsg.ParseFromString(msg.pbMsg);
  auto &itp_msg = itpMsg.itp_msg();

  auto tradeAccount = reinterpret_cast<CThostFtdcTradingAccountField *>(itp_msg.address());
  auto &traderSer = TraderSevice::getInstance();

  if (itp_msg.request_id() == 0) {
    manage_trader::message rsp;
    auto *accountRsp = rsp.mutable_account_status_rsp();
    accountRsp->set_result(manage_trader::Result::success);
    accountRsp->set_balance(tradeAccount->Balance);
    accountRsp->set_available(tradeAccount->Available);

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pbMsg);
    msg.sessionName = "manage_trader";
    msg.msgName = "AccountStatusRsp.0000000000";
    auto &recerSender = RecerSender::getInstance();
    recerSender.ROLE(Sender).ROLE(ProxySender).send(msg);
  } else {
    strategy_trader::message rsp;
    auto *accountRsp = rsp.mutable_account_status_rsp();
    accountRsp->set_result(strategy_trader::Result::success);
    accountRsp->set_balance(tradeAccount->Balance);
    accountRsp->set_available(tradeAccount->Available);

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pbMsg);
    msg.sessionName = "strategy_trader";
    msg.msgName = "AccountStatusRsp." + std::to_string(itp_msg.request_id());
    auto &recerSender = RecerSender::getInstance();
    recerSender.ROLE(Sender).ROLE(ProxySender).send(msg);
  }
}

void CtpEvent::OnRspQryInstrumentHandle(utils::ItpMsg &msg) {
  ipc::message itpMsg;
  itpMsg.ParseFromString(msg.pbMsg);
  auto &itp_msg = itpMsg.itp_msg();

  auto instrumentField = reinterpret_cast<CThostFtdcInstrumentField *>(itp_msg.address());

  if (itp_msg.request_id() < 1000000000) {
    market_trader::message rsp;
    auto *instrumentRsp = rsp.mutable_qry_instrument_rsp();

    instrumentRsp->set_instrument_id(instrumentField->InstrumentID);
    instrumentRsp->set_exchange_id(instrumentField->ExchangeID);
    instrumentRsp->set_price_tick(instrumentField->PriceTick);
    instrumentRsp->set_result(market_trader::Result::success);
    instrumentRsp->set_finish_flag(itp_msg.is_last());

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pbMsg);
    msg.sessionName = "market_trader";
    msg.msgName = "QryInstrumentRsp";
    auto &recerSender = RecerSender::getInstance();
    recerSender.ROLE(Sender).ROLE(ProxySender).send(msg);
  } else {
    strategy_trader::message rsp;
    auto *instrumentRsp = rsp.mutable_instrument_rsp();

    instrumentRsp->set_result(strategy_trader::Result::success);
    instrumentRsp->set_is_trading(instrumentField->IsTrading);
    instrumentRsp->set_max_limit_order_volume(instrumentField->MaxLimitOrderVolume);
    instrumentRsp->set_max_market_order_volume(instrumentField->MaxMarketOrderVolume);
    instrumentRsp->set_min_limit_order_volume(instrumentField->MinLimitOrderVolume);
    instrumentRsp->set_min_market_order_volume(instrumentField->MinMarketOrderVolume);
    instrumentRsp->set_price_tick(instrumentField->PriceTick);
    instrumentRsp->set_volume_multiple(instrumentField->VolumeMultiple);

    rsp.SerializeToString(&msg.pbMsg);
    msg.sessionName = "strategy_trader";
    msg.msgName = "InstrumentRsp." + std::to_string(itp_msg.request_id());
    auto &recerSender = RecerSender::getInstance();
    recerSender.ROLE(Sender).ROLE(ProxySender).send(msg);
  }
}

void CtpEvent::OnRspQryInstrumentMarginRateHandle(utils::ItpMsg &msg) {
  ipc::message itpMsg;
  itpMsg.ParseFromString(msg.pbMsg);
  auto &itp_msg = itpMsg.itp_msg();

  auto marginRateField = reinterpret_cast<CThostFtdcInstrumentMarginRateField *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *marginRateRsp = rsp.mutable_margin_rate_rsp();

  marginRateRsp->set_result(strategy_trader::Result::success);
  marginRateRsp->set_longmarginratiobymoney(marginRateField->LongMarginRatioByMoney);
  marginRateRsp->set_longmarginratiobyvolume(marginRateField->LongMarginRatioByVolume);
  marginRateRsp->set_shortmarginratiobymoney(marginRateField->ShortMarginRatioByMoney);
  marginRateRsp->set_shortmarginratiobyvolume(marginRateField->ShortMarginRatioByVolume);

  utils::ItpMsg sendmsg;
  rsp.SerializeToString(&sendmsg.pbMsg);
  sendmsg.sessionName = "strategy_trader";
  sendmsg.msgName = "MarginRateRsp." + std::to_string(itp_msg.request_id());
  auto &recerSender = RecerSender::getInstance();
  recerSender.ROLE(Sender).ROLE(ProxySender).send(sendmsg);
}

void CtpEvent::OnRspQryInstrumentCommissionRateHandle(utils::ItpMsg &msg) {
  ipc::message itpMsg;
  itpMsg.ParseFromString(msg.pbMsg);
  auto &itp_msg = itpMsg.itp_msg();

  auto commissionRateField = reinterpret_cast<CThostFtdcInstrumentCommissionRateField *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *commissionRateRsp = rsp.mutable_commission_rate_rsp();
  auto &traderSer = TraderSevice::getInstance();

  commissionRateRsp->set_result(strategy_trader::Result::success);
  commissionRateRsp->set_openratiobymoney(commissionRateField->CloseRatioByMoney);
  commissionRateRsp->set_openratiobyvolume(commissionRateField->CloseRatioByVolume);
  commissionRateRsp->set_closeratiobymoney(commissionRateField->CloseTodayRatioByMoney);
  commissionRateRsp->set_closeratiobyvolume(commissionRateField->CloseTodayRatioByVolume);
  commissionRateRsp->set_closetodayratiobymoney(commissionRateField->OpenRatioByMoney);
  commissionRateRsp->set_closetodayratiobyvolume(commissionRateField->OpenRatioByVolume);

  utils::ItpMsg sendmsg;
  rsp.SerializeToString(&sendmsg.pbMsg);
  sendmsg.sessionName = "strategy_trader";
  sendmsg.msgName = "CommissionRateRsp." + std::to_string(itp_msg.request_id());
  auto &recerSender = RecerSender::getInstance();
  recerSender.ROLE(Sender).ROLE(ProxySender).send(sendmsg);
}

bool CtpEvent::sendEmail(const utils::OrderContent &content) {
  std::string subjectContent = "";
  subjectContent += content.instrumentID + "成交通知";

  std::string saveContent = "";
  saveContent += "账户: " + content.userId + "\n";
  saveContent += "合约: " + content.instrumentID + "\n";
  saveContent += "下单价格: " + std::to_string(content.limit_price) + "\n";
  saveContent += "成交价格: " + std::to_string(content.tradedOrder.price) + "\n";
  saveContent += "成交日期: " + content.tradedOrder.date + "\n";
  saveContent += "成交时间: " + content.tradedOrder.time + "\n";
  std::string direction = (content.tradedOrder.direction == "0") ? "BUY" : "SELL";
  saveContent += "方向: " + direction + "\n";
  saveContent += "下单数量: " + std::to_string(content.total_volume) + "\n";
  saveContent += "本批成交数量: " + std::to_string(content.tradedOrder.volume) + "\n";

  auto &recerSender = RecerSender::getInstance();
  recerSender.ROLE(Sender).ROLE(EmailSender).send(subjectContent.c_str(), saveContent.c_str());

  return true;
}
