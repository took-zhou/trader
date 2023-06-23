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
#include "common/self/file_util.h"
#include "common/self/profiler.h"
#include "common/self/protobuf/ipc.pb.h"
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
  msg_func_map_["OnRspOrderInsert"] = [this](utils::ItpMsg &msg) { OnRspOrderInsertHandle(msg); };
  msg_func_map_["OnRspOrderAction"] = [this](utils::ItpMsg &msg) { OnRspOrderActionHandle(msg); };
  msg_func_map_["OnRspQryInstrument"] = [this](utils::ItpMsg &msg) { OnRspQryInstrumentHandle(msg); };
  msg_func_map_["OnRspQryTradingAccount"] = [this](utils::ItpMsg &msg) { OnRspQryTradingAccountHandle(msg); };
  msg_func_map_["OnRspQryInstrumentMarginRate"] = [this](utils::ItpMsg &msg) { OnRspQryInstrumentMarginRateHandle(msg); };
  msg_func_map_["OnRspQryInstrumentCommissionRate"] = [this](utils::ItpMsg &msg) { OnRspQryInstrumentCommissionRateHandle(msg); };
  msg_func_map_["OnRspQryOptionInstrCommRate"] = [this](utils::ItpMsg &msg) { OnRspQryOptionInstrCommRateHandle(msg); };

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
      ScopedTimer timer("OnRtnOrderHandle");
#endif
      auto &recer_sender = RecerSender::GetInstance();
      if (content->active_cancle_indication) {
        strategy_trader::message message;
        auto *order_cancel_rsp = message.mutable_order_cancel_rsp();
        order_cancel_rsp->set_instrument(content->instrument_id);
        order_cancel_rsp->set_index(content->index);
        order_cancel_rsp->set_result(strategy_trader::Result::success);

        utils::ItpMsg msg;
        message.SerializeToString(&msg.pb_msg);
        msg.session_name = "strategy_trader";
        msg.msg_name = "OrderCancelRsp";
        recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
      }
      strategy_trader::message rsp;
      auto *insert_rsp = rsp.mutable_order_insert_rsp();
      insert_rsp->set_instrument(content->instrument_id);
      insert_rsp->set_index(content->index);
      insert_rsp->set_result(strategy_trader::Result::failed);
      if (content->active_cancle_indication) {
        insert_rsp->set_reason(strategy_trader::FailedReason::Order_Cancel);
      } else {
        insert_rsp->set_reason(strategy_trader::FailedReason::No_Trading_Time);
      }
      rsp.SerializeToString(&msg.pb_msg);
      msg.session_name = "strategy_trader";
      msg.msg_name = "OrderInsertRsp";

      INFO_LOG("the order be canceled, order ref: %s.", order->OrderRef);
      order_manage.DelOrder(order->OrderRef);
      recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
    }
  }
}

void CtpEvent::OnRtnTradeHandle(utils::ItpMsg &msg) {
  PZone("OnRtnTradeHandle");
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto trade = reinterpret_cast<CThostFtdcTradeField *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto &order_lookup = trader_ser.ROLE(OrderLookup);

#ifdef BENCH_TEST
  ScopedTimer t("OnRtnTradeHandle");
#endif
  auto content = order_manage.GetOrder(trade->OrderRef);
  if (content != nullptr) {
    PZone("SendResult");
    content->traded_order.price = trade->Price;
    content->traded_order.volume = trade->Volume;
    // content->traded_order.volume = content->total_volume;
    if (trade->Direction == '0') {
      content->traded_order.direction = 1;
    } else {
      content->traded_order.direction = 2;
    }
    if (trade->OffsetFlag == '0') {
      content->traded_order.comboffset = 1;
    } else {
      content->traded_order.comboffset = 2;
    }
    content->traded_order.date = trade->TradeDate;
    content->traded_order.time = trade->TradeTime;
    content->left_volume -= trade->Volume;

    strategy_trader::message rsp;
    auto *insert_rsp = rsp.mutable_order_insert_rsp();
    insert_rsp->set_instrument(content->instrument_id);
    insert_rsp->set_index(content->index);
    insert_rsp->set_result(strategy_trader::Result::success);

    auto *succ_info = insert_rsp->mutable_info();
    succ_info->set_orderprice(content->traded_order.price);
    succ_info->set_ordervolume(content->traded_order.volume);

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";

    if (content->left_volume == 0) {
      if (content->comboffset != 1) {
        std::string temp_key;
        temp_key += content->instrument_id;
        temp_key += ".";
        temp_key += content->index;
        order_lookup.DelOrderIndex(temp_key);
      }
      auto &json_cfg = utils::JsonConfig::GetInstance();
      auto send_email = json_cfg.GetConfig("trader", "SendOrderEmail").get<std::string>();
      if (send_email == "send") {
        SendEmail(*content);
      }
      INFO_LOG("the order was finished, order ref: %s.", trade->OrderRef);
      order_manage.DelOrder(trade->OrderRef);
    }
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
  }
}

void CtpEvent::OnRspOrderInsertHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order_insert_rsp = reinterpret_cast<CThostFtdcInputOrderField *>(itp_msg.address());
  auto rsp_info = reinterpret_cast<CThostFtdcRspInfoField *>(itp_msg.rsp_info());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(order_insert_rsp->OrderRef);
  if (content != nullptr) {
    strategy_trader::message message;
    auto *insert_rsp = message.mutable_order_insert_rsp();
    insert_rsp->set_instrument(content->instrument_id);
    insert_rsp->set_index(content->index);
    insert_rsp->set_result(strategy_trader::Result::failed);
    if (rsp_info->ErrorID == 31) {
      insert_rsp->set_reason(strategy_trader::FailedReason::Fund_Shortage_Error);
    } else if (rsp_info->ErrorID == 50 || rsp_info->ErrorID == 51 || rsp_info->ErrorID == 30) {
      insert_rsp->set_reason(strategy_trader::FailedReason::No_Opened_Order);
    } else {
      insert_rsp->set_reason(strategy_trader::FailedReason::Order_Fill_Error);
    }

    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";

    INFO_LOG("the order be canceled, orderRef: %s.", order_insert_rsp->OrderRef);
    order_manage.DelOrder(order_insert_rsp->OrderRef);
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
  } else {
    ERROR_LOG("not find order ref: %s", order_insert_rsp->OrderRef);
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
    order_cancel_rsp->set_instrument(content->instrument_id);
    order_cancel_rsp->set_index(content->index);
    order_cancel_rsp->set_result(strategy_trader::Result::failed);
    order_cancel_rsp->set_failedreason("INVALID");

    utils::ItpMsg msg;
    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderCancelRsp";
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
  } else {
    ERROR_LOG("not find order ref: %s", order_action_rsp->OrderRef);
  }
}

void CtpEvent::OnRspQryTradingAccountHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto account = reinterpret_cast<CThostFtdcTradingAccountField *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  trader_ser.ROLE(AccountAssign).UpdateAccountStatus(account->Balance, account->Available, itp_msg.session_id(), itp_msg.user_id());
}

void CtpEvent::OnRspQryInstrumentHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto instrument_field = reinterpret_cast<CThostFtdcInstrumentField *>(itp_msg.address());

  market_trader::message rsp;
  auto *instrument_rsp = rsp.mutable_qry_instrument_rsp();

  instrument_rsp->set_instrument_id(instrument_field->InstrumentID);
  instrument_rsp->set_exchange_id(instrument_field->ExchangeID);
  instrument_rsp->set_ticksize(instrument_field->PriceTick);
  instrument_rsp->set_tradeuint(instrument_field->VolumeMultiple);
  instrument_rsp->set_is_trade(instrument_field->IsTrading);
  instrument_rsp->set_max_limit_volume(instrument_field->MaxLimitOrderVolume);
  instrument_rsp->set_max_market_volume(instrument_field->MaxMarketOrderVolume);
  instrument_rsp->set_min_limit_volume(instrument_field->MinLimitOrderVolume);
  instrument_rsp->set_min_market_volume(instrument_field->MinMarketOrderVolume);
  instrument_rsp->set_result(market_trader::Result::success);
  instrument_rsp->set_finish_flag(itp_msg.is_last());

  utils::ItpMsg ret_msg;
  rsp.SerializeToString(&ret_msg.pb_msg);
  ret_msg.session_name = "market_trader";
  ret_msg.msg_name = "QryInstrumentRsp";
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(ret_msg);
}

void CtpEvent::OnRspQryInstrumentMarginRateHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto margin_rate_field = reinterpret_cast<CThostFtdcInstrumentMarginRateField *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *margin_rate_rsp = rsp.mutable_margin_rate_rsp();

  margin_rate_rsp->set_exchange_id(margin_rate_field->ExchangeID);
  margin_rate_rsp->set_instrument_id(margin_rate_field->InstrumentID);
  margin_rate_rsp->set_user_id(itp_msg.user_id());
  margin_rate_rsp->set_result(strategy_trader::Result::success);
  margin_rate_rsp->set_longmarginratiobymoney(margin_rate_field->LongMarginRatioByMoney);
  margin_rate_rsp->set_longmarginratiobyvolume(margin_rate_field->LongMarginRatioByVolume);
  margin_rate_rsp->set_shortmarginratiobymoney(margin_rate_field->ShortMarginRatioByMoney);
  margin_rate_rsp->set_shortmarginratiobyvolume(margin_rate_field->ShortMarginRatioByVolume);

  utils::ItpMsg sendmsg;
  rsp.SerializeToString(&sendmsg.pb_msg);
  sendmsg.session_name = "strategy_trader";
  sendmsg.msg_name = "MarginRateRsp";
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(sendmsg);
}

void CtpEvent::OnRspQryInstrumentCommissionRateHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto commission_rate_field = reinterpret_cast<CThostFtdcInstrumentCommissionRateField *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *commission_rate_rsp = rsp.mutable_commission_rate_rsp();

  commission_rate_rsp->set_exchange_id(commission_rate_field->ExchangeID);
  commission_rate_rsp->set_instrument_id(commission_rate_field->InstrumentID);
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
  sendmsg.msg_name = "CommissionRateRsp";
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(sendmsg);
}

void CtpEvent::OnRspQryOptionInstrCommRateHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto commission_rate_field = reinterpret_cast<CThostFtdcOptionInstrCommRateField *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *commission_rate_rsp = rsp.mutable_commission_rate_rsp();

  commission_rate_rsp->set_exchange_id(commission_rate_field->ExchangeID);
  commission_rate_rsp->set_instrument_id(commission_rate_field->InstrumentID);
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
  sendmsg.msg_name = "CommissionRateRsp";
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(sendmsg);
}

bool CtpEvent::SendEmail(const utils::OrderContent &content) {
  char subject_content[30];
  char save_content[180];
  sprintf(subject_content, "%s transaction notice", content.instrument_id.c_str());

  sprintf(save_content, "account: %s\ninstrument: %s\norder price: %f\ndirection: %s\ncomboffset: %s\norder volume: %d",
          content.user_id.c_str(), content.instrument_id.c_str(), content.limit_price, content.direction == 1 ? "BUY" : "SELL",
          content.comboffset == 1 ? "OPEN" : "CLOSE", content.total_volume);

  auto &recer_sender = RecerSender::GetInstance();
  ipc::message send_message;
  auto *send_email = send_message.mutable_send_email();
  send_email->set_head(subject_content);
  send_email->set_body(save_content);

  utils::ItpMsg itp_msg;
  send_message.SerializeToString(&itp_msg.pb_msg);
  itp_msg.session_name = "trader_trader";
  itp_msg.msg_name = "SendEmail";
  // innerSenders专为itp设计，所以只能走ProxySender的接口
  recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(itp_msg);

  return true;
}
