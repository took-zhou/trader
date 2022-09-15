/*
 * ctpEvent.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/interface/ctp_event/ctp_event.h"
#include <algorithm>
#include <mutex>
#include "common/extern/log/log.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/protobuf/manage-trader.pb.h"
#include "common/self/protobuf/market-trader.pb.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

CtpEvent::CtpEvent() { RegMsgFun(); }

void CtpEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map_.clear();
  msg_func_map_["OnRtnOrder"] = [this](utils::ItpMsg &msg) { OnRtnOrderHandle(msg); };
  msg_func_map_["OnRtnTrade"] = [this](utils::ItpMsg &msg) { OnRtnTradeHandle(msg); };
  msg_func_map_["OnRspOrderAction"] = [this](utils::ItpMsg &msg) { OnRspOrderActionHandle(msg); };
  msg_func_map_["OnRspQryInstrument"] = [this](utils::ItpMsg &msg) { OnRspQryInstrumentHandle(msg); };
  msg_func_map_["OnRspQryTradingAccount"] = [this](utils::ItpMsg &msg) { OnRspQryTradingAccountHandle(msg); };
  msg_func_map_["OnRspQryInstrumentMarginRate"] = [this](utils::ItpMsg &msg) { OnRspQryInstrumentMarginRateHandle(msg); };
  msg_func_map_["OnRspQryInstrumentCommissionRate"] = [this](utils::ItpMsg &msg) { OnRspQryInstrumentCommissionRateHandle(msg); };

  for (auto &iter : msg_func_map_) {
    INFO_LOG("msg_func_map_[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void CtpEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map_.find(msg.msg_name);
  if (iter != msg_func_map_.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msg_name [%s]!", msg.msg_name.c_str());
  return;
}

void CtpEvent::OnRtnOrderHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order = reinterpret_cast<CThostFtdcOrderField *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(order->OrderRef);
  if (content != nullptr) {
    if (order->OrderStatus == THOST_FTDC_OST_Canceled) {
#ifdef BENCH_TEST
      ScopedTimer t("OnRtnOrderHandle");
#endif
      auto &recer_sender = RecerSender::GetInstance();
      {
        strategy_trader::message message;
        auto *order_cancel_rsp = message.mutable_order_cancel_rsp();
        order_cancel_rsp->set_order_ref(order->OrderRef);
        order_cancel_rsp->set_result(strategy_trader::Result::success);

        utils::ItpMsg msg;
        message.SerializeToString(&msg.pb_msg);
        msg.session_name = "strategy_trader";
        msg.msg_name = "OrderCancelRsp." + content->prid;
        recer_sender.ROLE(Sender).ROLE(ProxySender).Send(msg);
      }

      {
        strategy_trader::message message;
        auto *insert_rsp = message.mutable_order_insert_rsp();
        insert_rsp->set_order_ref(content->order_ref);
        insert_rsp->set_result(strategy_trader::Result::failed);
        insert_rsp->set_reason(strategy_trader::FailedReason::Order_Cancel);

        message.SerializeToString(&msg.pb_msg);
        msg.session_name = "strategy_trader";
        msg.msg_name = "OrderInsertRsp." + content->prid;
        recer_sender.ROLE(Sender).ROLE(ProxySender).Send(msg);
      }

      INFO_LOG("the order be canceled, order ref: %s, prid: %s.", order->OrderRef, content->prid.c_str());
      order_manage.DelOrder(order->OrderRef);
    }
  }
}

void CtpEvent::OnRtnTradeHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto trade = reinterpret_cast<CThostFtdcTradeField *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);

#ifdef BENCH_TEST
  ScopedTimer t("OnRtnTradeHandle");
#endif
  auto content = order_manage.GetOrder(trade->OrderRef);
  if (content != nullptr) {
    content->traded_order.price = trade->Price;
    content->traded_order.volume = trade->Volume;
    content->traded_order.direction = std::to_string(trade->Direction);
    content->traded_order.date = trade->TradeDate;
    content->traded_order.time = trade->TradeTime;

    strategy_trader::message rsp;
    auto *insert_rsp = rsp.mutable_order_insert_rsp();
    insert_rsp->set_order_ref(trade->OrderRef);
    insert_rsp->set_result(strategy_trader::Result::success);

    auto *succ_info = insert_rsp->mutable_info();
    succ_info->set_orderprice(content->traded_order.price);
    succ_info->set_ordervolume(content->traded_order.volume);

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp." + content->prid;
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(ProxySender).Send(msg);

    SendEmail(*content);

    content->left_volume -= trade->Volume;
    if (content->left_volume == 0) {
      INFO_LOG("the order was finished, order ref: %s, prid: %s.", trade->OrderRef, content->prid.c_str());
      order_manage.DelOrder(trade->OrderRef);
    }
  }
}

void CtpEvent::OnRspOrderActionHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order_action_rsp = reinterpret_cast<CThostFtdcInputOrderActionField *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(order_action_rsp->OrderRef);
  if (content != nullptr) {
    strategy_trader::message message;
    auto *order_cancel_rsp = message.mutable_order_cancel_rsp();
    order_cancel_rsp->set_order_ref(order_action_rsp->OrderRef);
    order_cancel_rsp->set_result(strategy_trader::Result::failed);
    order_cancel_rsp->set_failedreason("INVALID");

    utils::ItpMsg msg;
    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderCancelRsp." + content->prid;
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(ProxySender).Send(msg);
  }
}

void CtpEvent::OnRspQryTradingAccountHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto trade_account = reinterpret_cast<CThostFtdcTradingAccountField *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();

  if (itp_msg.request_id() == 0) {
    manage_trader::message message;
    auto *account_rsp = message.mutable_account_status_rsp();
    account_rsp->set_result(manage_trader::Result::success);
    account_rsp->set_user_id(itp_msg.user_id());
    account_rsp->set_session_id(itp_msg.session_id());
    account_rsp->set_balance(trade_account->Balance);
    account_rsp->set_available(trade_account->Available);

    utils::ItpMsg msg;
    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "manage_trader";
    msg.msg_name = "AccountStatusRsp.0000000000";
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(ProxySender).Send(msg);
  } else {
    strategy_trader::message rsp;
    auto *account_rsp = rsp.mutable_account_status_rsp();
    account_rsp->set_result(strategy_trader::Result::success);
    account_rsp->set_user_id(itp_msg.user_id());
    account_rsp->set_session_id(itp_msg.session_id());
    account_rsp->set_balance(trade_account->Balance);
    account_rsp->set_available(trade_account->Available);

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "AccountStatusRsp." + std::to_string(itp_msg.request_id());
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(ProxySender).Send(msg);
  }
}

void CtpEvent::OnRspQryInstrumentHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto instrument_field = reinterpret_cast<CThostFtdcInstrumentField *>(itp_msg.address());

  if (itp_msg.request_id() < 1000000000) {
    market_trader::message rsp;
    auto *instrument_rsp = rsp.mutable_qry_instrument_rsp();

    instrument_rsp->set_instrument_id(instrument_field->InstrumentID);
    instrument_rsp->set_exchange_id(instrument_field->ExchangeID);
    instrument_rsp->set_price_tick(instrument_field->PriceTick);
    instrument_rsp->set_result(market_trader::Result::success);
    instrument_rsp->set_finish_flag(itp_msg.is_last());

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pb_msg);
    msg.session_name = "market_trader";
    msg.msg_name = "QryInstrumentRsp";
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(ProxySender).Send(msg);
  } else {
    strategy_trader::message rsp;
    auto *instrument_rsp = rsp.mutable_instrument_rsp();

    instrument_rsp->set_result(strategy_trader::Result::success);
    instrument_rsp->set_is_trading(instrument_field->IsTrading);
    instrument_rsp->set_max_limit_order_volume(instrument_field->MaxLimitOrderVolume);
    instrument_rsp->set_max_market_order_volume(instrument_field->MaxMarketOrderVolume);
    instrument_rsp->set_min_limit_order_volume(instrument_field->MinLimitOrderVolume);
    instrument_rsp->set_min_market_order_volume(instrument_field->MinMarketOrderVolume);
    instrument_rsp->set_price_tick(instrument_field->PriceTick);
    instrument_rsp->set_volume_multiple(instrument_field->VolumeMultiple);

    rsp.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "InstrumentRsp." + std::to_string(itp_msg.request_id());
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(ProxySender).Send(msg);
  }
}

void CtpEvent::OnRspQryInstrumentMarginRateHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto margin_rate_field = reinterpret_cast<CThostFtdcInstrumentMarginRateField *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *margin_rate_rsp = rsp.mutable_margin_rate_rsp();

  margin_rate_rsp->set_user_id(itp_msg.user_id());
  margin_rate_rsp->set_result(strategy_trader::Result::success);
  margin_rate_rsp->set_longmarginratiobymoney(margin_rate_field->LongMarginRatioByMoney);
  margin_rate_rsp->set_longmarginratiobyvolume(margin_rate_field->LongMarginRatioByVolume);
  margin_rate_rsp->set_shortmarginratiobymoney(margin_rate_field->ShortMarginRatioByMoney);
  margin_rate_rsp->set_shortmarginratiobyvolume(margin_rate_field->ShortMarginRatioByVolume);

  utils::ItpMsg sendmsg;
  rsp.SerializeToString(&sendmsg.pb_msg);
  sendmsg.session_name = "strategy_trader";
  sendmsg.msg_name = "MarginRateRsp." + std::to_string(itp_msg.request_id());
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).Send(sendmsg);
}

void CtpEvent::OnRspQryInstrumentCommissionRateHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto commission_rate_field = reinterpret_cast<CThostFtdcInstrumentCommissionRateField *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *commission_rate_rsp = rsp.mutable_commission_rate_rsp();
  auto &trader_ser = TraderSevice::GetInstance();

  commission_rate_rsp->set_user_id(itp_msg.user_id());
  commission_rate_rsp->set_result(strategy_trader::Result::success);
  commission_rate_rsp->set_openratiobymoney(commission_rate_field->CloseRatioByMoney);
  commission_rate_rsp->set_openratiobyvolume(commission_rate_field->CloseRatioByVolume);
  commission_rate_rsp->set_closeratiobymoney(commission_rate_field->CloseTodayRatioByMoney);
  commission_rate_rsp->set_closeratiobyvolume(commission_rate_field->CloseTodayRatioByVolume);
  commission_rate_rsp->set_closetodayratiobymoney(commission_rate_field->OpenRatioByMoney);
  commission_rate_rsp->set_closetodayratiobyvolume(commission_rate_field->OpenRatioByVolume);

  utils::ItpMsg sendmsg;
  rsp.SerializeToString(&sendmsg.pb_msg);
  sendmsg.session_name = "strategy_trader";
  sendmsg.msg_name = "CommissionRateRsp." + std::to_string(itp_msg.request_id());
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).Send(sendmsg);
}

bool CtpEvent::SendEmail(const utils::OrderContent &content) {
  std::string subject_content = "";
  subject_content += content.instrument_id + "成交通知";

  std::string save_content = "";
  save_content += "账户: " + content.user_id + "\n";
  save_content += "合约: " + content.instrument_id + "\n";
  save_content += "下单价格: " + std::to_string(content.limit_price) + "\n";
  save_content += "成交价格: " + std::to_string(content.traded_order.price) + "\n";
  save_content += "成交日期: " + content.traded_order.date + "\n";
  save_content += "成交时间: " + content.traded_order.time + "\n";
  std::string direction = (content.traded_order.direction == "0") ? "BUY" : "SELL";
  save_content += "方向: " + direction + "\n";
  save_content += "下单数量: " + std::to_string(content.total_volume) + "\n";
  save_content += "本批成交数量: " + std::to_string(content.traded_order.volume) + "\n";

  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(EmailSender).Send(subject_content.c_str(), save_content.c_str());

  return true;
}
