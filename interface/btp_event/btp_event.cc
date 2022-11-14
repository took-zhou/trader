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
        order_cancel_rsp->set_order_ref(std::to_string(order_info->order_ref));
        order_cancel_rsp->set_result(strategy_trader::Result::success);

        utils::ItpMsg msg;
        message.SerializeToString(&msg.pb_msg);
        msg.session_name = "strategy_trader";
        msg.msg_name = "OrderCancelRsp." + content->prid;
        recer_sender.ROLE(Sender).ROLE(ProxySender).Send(msg);
      }
      strategy_trader::message rsp;
      auto *insert_rsp = rsp.mutable_order_insert_rsp();
      insert_rsp->set_order_ref(content->order_ref);
      insert_rsp->set_result(strategy_trader::Result::failed);
      insert_rsp->set_reason(strategy_trader::FailedReason::Order_Cancel);
      rsp.SerializeToString(&msg.pb_msg);
      msg.session_name = "strategy_trader";
      msg.msg_name = "OrderInsertRsp." + content->prid;
      recer_sender.ROLE(Sender).ROLE(ProxySender).Send(msg);
      INFO_LOG("the order be canceled, orderRef: %d, prid: %s.", order_info->order_ref, content->prid.c_str());

      order_manage.DelOrder(std::to_string(order_info->order_ref));
    }
  } else {
    ERROR_LOG("not find order ref: %d", order_info->order_ref);
  }
}

void BtpEvent::OnRtnTradeHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto trade_report = reinterpret_cast<BtpOrderInfoStruct *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);

#ifdef BENCH_TEST
  ScopedTimer timer("OnRtnTradeHandle");
#endif
  auto content = order_manage.GetOrder(std::to_string(trade_report->order_ref));
  if (content != nullptr) {
    content->traded_order.price = trade_report->price;
    content->traded_order.volume = trade_report->volume;
    content->traded_order.direction = std::to_string(trade_report->side);
    content->traded_order.date = trade_report->date;
    content->traded_order.time = trade_report->time;

    strategy_trader::message rsp;
    auto *insert_rsp = rsp.mutable_order_insert_rsp();
    insert_rsp->set_order_ref(std::to_string(trade_report->order_ref));
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

    content->left_volume -= trade_report->volume;
    if (content->left_volume == 0) {
      INFO_LOG("the order was finished, ref[%d],identity[%s]", trade_report->order_ref, content->prid.c_str());
      order_manage.DelOrder(std::to_string(trade_report->order_ref));
    }
  } else {
    ERROR_LOG("not find order ref: %d", trade_report->order_ref);
  }
}

void BtpEvent::OnRspTradingAccountHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto account_info = reinterpret_cast<BtpAccountInfo *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();

  strategy_trader::message rsp;
  auto *account_rsp = rsp.mutable_account_status_rsp();
  account_rsp->set_result(strategy_trader::Result::success);
  account_rsp->set_user_id(itp_msg.user_id());
  account_rsp->set_session_id(itp_msg.session_id());
  account_rsp->set_balance(account_info->balance);
  account_rsp->set_available(account_info->available);

  utils::ItpMsg send_msg;
  rsp.SerializeToString(&send_msg.pb_msg);
  send_msg.session_name = "strategy_trader";
  send_msg.msg_name = "AccountStatusRsp." + std::to_string(itp_msg.request_id());
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).Send(send_msg);
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
  sendmsg.msg_name = "MarginRateRsp." + std::to_string(itp_msg.request_id());
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).Send(sendmsg);
}

void BtpEvent::OnRspCommissionRateHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto commission_info = reinterpret_cast<BtpCommissionInfo *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *commission_rate_rsp = rsp.mutable_commission_rate_rsp();
  auto &trader_ser = TraderSevice::GetInstance();

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
  sendmsg.msg_name = "CommissionRateRsp." + std::to_string(itp_msg.request_id());
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).Send(sendmsg);
}

bool BtpEvent::SendEmail(const utils::OrderContent &content) {
  char subject_content[30];
  char save_content[200];
  sprintf(subject_content, "%s transaction notice", content.instrument_id.c_str());

  if (content.traded_order.direction == "1") {
    sprintf(save_content,
            "account: %s\ninstrument: %s\norder price: %f\ntransaction price: "
            "%f\ndate: %s\ntime: %s\ndirection: BUY\norder volume: "
            "%d\ntransaction volume: %d",
            content.user_id.c_str(), content.instrument_id.c_str(), content.limit_price, content.traded_order.price,
            content.traded_order.date.c_str(), content.traded_order.time.c_str(), content.total_volume, content.traded_order.volume);
  } else {
    sprintf(save_content,
            "account: %s\ninstrument: %s\norder price: %f\ntransaction price: "
            "%f\ndate: %s\ntime: %s\ndirection: SELL\norder volume: "
            "%d\ntransaction volume: %d",
            content.user_id.c_str(), content.instrument_id.c_str(), content.limit_price, content.traded_order.price,
            content.traded_order.date.c_str(), content.traded_order.time.c_str(), content.total_volume, content.traded_order.volume);
  }

  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(EmailSender).Send(subject_content, save_content);

  return true;
}
