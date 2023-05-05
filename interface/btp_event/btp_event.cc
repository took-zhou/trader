/*
 * ctpEvent.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/interface/btp_event/btp_event.h"
#include <algorithm>
#include <mutex>
#include "common/extern/btp/inc/btp_trader_api.h"
#include "common/extern/log/log.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/protobuf/market-trader.pb.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

BtpEvent::BtpEvent() { RegMsgFun(); }

void BtpEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map_.clear();
  msg_func_map_["OnRtnOrder"] = [this](utils::ItpMsg &msg) { OnRtnOrderHandle(msg); };
  msg_func_map_["OnRtnTrade"] = [this](utils::ItpMsg &msg) { OnRtnTradeHandle(msg); };
  msg_func_map_["OnRtnOrderInsert"] = [this](utils::ItpMsg &msg) { OnRtnOrderInsertHandle(msg); };
  msg_func_map_["OnRtnOrderAction"] = [this](utils::ItpMsg &msg) { OnRtnOrderActionHandle(msg); };
  msg_func_map_["OnRspTradingAccount"] = [this](utils::ItpMsg &msg) { OnRspTradingAccountHandle(msg); };
  msg_func_map_["OnRspMarginRate"] = [this](utils::ItpMsg &msg) { OnRspMarginRateHandle(msg); };
  msg_func_map_["OnRspCommissionRate"] = [this](utils::ItpMsg &msg) { OnRspCommissionRateHandle(msg); };

  for (auto &iter : msg_func_map_) {
    INFO_LOG("msg_func_map_[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void BtpEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map_.find(msg.msg_name);
  if (iter != msg_func_map_.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msg_name.c_str());
  return;
}

void BtpEvent::OnRtnOrderHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order_info = reinterpret_cast<BtpOrderInfoStruct *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(std::to_string(order_info->order_ref));
  if (content != nullptr) {
    if (order_info->order_status == kCanceled || order_info->order_status == kRejected) {
#ifdef BENCH_TEST
      ScopedTimer t("OnRtnOrderHandle");
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
      insert_rsp->set_reason(strategy_trader::FailedReason::Order_Cancel);
      rsp.SerializeToString(&msg.pb_msg);
      msg.session_name = "strategy_trader";
      msg.msg_name = "OrderInsertRsp";
      recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
      INFO_LOG("the order be canceled, orderRef: %d.", order_info->order_ref);

      order_manage.DelOrder(std::to_string(order_info->order_ref));
    }
  }
}

void BtpEvent::OnRtnTradeHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto trade_report = reinterpret_cast<BtpOrderInfoStruct *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto &order_lookup = trader_ser.ROLE(OrderLookup);

#ifdef BENCH_TEST
  ScopedTimer timer("OnRtnTradeHandle");
#endif
  auto content = order_manage.GetOrder(std::to_string(trade_report->order_ref));
  if (content != nullptr) {
    content->traded_order.price = trade_report->price;
    content->traded_order.volume = trade_report->volume;
    content->traded_order.direction = trade_report->side;
    content->traded_order.comboffset = trade_report->comboffset;
    content->traded_order.date = trade_report->date;
    content->traded_order.time = trade_report->time;

    strategy_trader::message rsp;
    auto *insert_rsp = rsp.mutable_order_insert_rsp();
    insert_rsp->set_result(strategy_trader::Result::success);
    insert_rsp->set_instrument(content->instrument_id);
    insert_rsp->set_index(content->index);

    auto *succ_info = insert_rsp->mutable_info();
    succ_info->set_orderprice(content->traded_order.price);
    succ_info->set_ordervolume(content->traded_order.volume);
    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);

    content->left_volume -= trade_report->volume;
    if (content->left_volume == 0) {
      if (content->comboffset != 1) {
        std::string temp_key;
        temp_key += content->instrument_id;
        temp_key += ".";
        temp_key += content->index;
        order_lookup.DelOrderIndex(temp_key);
      }
      SendEmail(*content);
      INFO_LOG("the order was finished, ref[%d].", trade_report->order_ref);
      order_manage.DelOrder(std::to_string(trade_report->order_ref));
    }
  }
}

void BtpEvent::OnRtnOrderInsertHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order_insert = reinterpret_cast<BtpOrderInfoStruct *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(std::to_string(order_insert->order_ref));
  if (content != nullptr) {
    strategy_trader::message message;
    auto *insert_rsp = message.mutable_order_insert_rsp();
    insert_rsp->set_instrument(content->instrument_id);
    insert_rsp->set_index(content->index);
    insert_rsp->set_result(strategy_trader::Result::failed);
    if (order_insert->order_status == kRejected) {
      insert_rsp->set_reason(strategy_trader::FailedReason::Fund_Shortage_Error);
    } else if (order_insert->order_status == kNoOpened) {
      insert_rsp->set_reason(strategy_trader::FailedReason::No_Opened_Order);
    }

    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
    INFO_LOG("the order be canceled, orderRef: %d.", order_insert->order_ref);

    order_manage.DelOrder(std::to_string(order_insert->order_ref));
  } else {
    ERROR_LOG("not find order ref: %d", order_insert->order_ref);
  }
}

void BtpEvent::OnRtnOrderActionHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order_action_rsp = reinterpret_cast<BtpOrderInfoStruct *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(std::to_string(order_action_rsp->order_ref));
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
    ERROR_LOG("not find order ref: %d", order_action_rsp->order_ref);
  }
}

void BtpEvent::OnRspTradingAccountHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto account = reinterpret_cast<BtpAccountInfo *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  trader_ser.ROLE(AccountAssign).UpdateAccountStatus(account->balance, account->available, itp_msg.session_id(), itp_msg.user_id());
}

void BtpEvent::OnRspMarginRateHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto margin_info = reinterpret_cast<BtpMarginInfo *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *margin_rate_rsp = rsp.mutable_margin_rate_rsp();

  margin_rate_rsp->set_exchange_id(margin_info->exchange_id);
  margin_rate_rsp->set_instrument_id(margin_info->instrument_id);
  margin_rate_rsp->set_user_id(itp_msg.user_id());
  margin_rate_rsp->set_result(strategy_trader::Result::success);
  margin_rate_rsp->set_longmarginratiobymoney(margin_info->long_margin_ratio_by_money);
  margin_rate_rsp->set_longmarginratiobyvolume(margin_info->long_margin_ratio_by_volume);
  margin_rate_rsp->set_shortmarginratiobymoney(margin_info->short_margin_ratio_by_money);
  margin_rate_rsp->set_shortmarginratiobyvolume(margin_info->short_margin_ratio_by_volume);

  utils::ItpMsg sendmsg;
  rsp.SerializeToString(&sendmsg.pb_msg);
  sendmsg.session_name = "strategy_trader";
  sendmsg.msg_name = "MarginRateRsp";
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(sendmsg);
}

void BtpEvent::OnRspCommissionRateHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto commission_info = reinterpret_cast<BtpCommissionInfo *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *commission_rate_rsp = rsp.mutable_commission_rate_rsp();

  commission_rate_rsp->set_exchange_id(commission_info->exchange_id);
  commission_rate_rsp->set_instrument_id(commission_info->instrument_id);
  commission_rate_rsp->set_user_id(itp_msg.user_id());
  commission_rate_rsp->set_result(strategy_trader::Result::success);
  commission_rate_rsp->set_openratiobymoney(commission_info->open_ratio_by_money);
  commission_rate_rsp->set_openratiobyvolume(commission_info->open_ratio_by_volume);
  commission_rate_rsp->set_closeratiobymoney(commission_info->close_ratio_by_money);
  commission_rate_rsp->set_closeratiobyvolume(commission_info->close_ratio_by_volume);
  commission_rate_rsp->set_closetodayratiobymoney(commission_info->close_today_ratio_by_money);
  commission_rate_rsp->set_closetodayratiobyvolume(commission_info->close_today_ratio_by_volume);

  utils::ItpMsg sendmsg;
  rsp.SerializeToString(&sendmsg.pb_msg);
  sendmsg.session_name = "strategy_trader";
  sendmsg.msg_name = "CommissionRateRsp";
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(sendmsg);
}

bool BtpEvent::SendEmail(const utils::OrderContent &content) {
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
