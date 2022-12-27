/*
 * ctpEvent.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/interface/xtp_event/xtp_event.h"
#include <algorithm>
#include <mutex>
#include "common/extern/log/log.h"
#include "common/extern/xtp/inc/xtp_trader_api.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/protobuf/market-trader.pb.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

XtpEvent::XtpEvent() { RegMsgFun(); }

void XtpEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map_.clear();
  msg_func_map_["OnOrderEvent"] = [this](utils::ItpMsg &msg) { OnOrderEventHandle(msg); };
  msg_func_map_["OnTradeEvent"] = [this](utils::ItpMsg &msg) { OnTradeEventHandle(msg); };
  msg_func_map_["OnCancelOrderError"] = [this](utils::ItpMsg &msg) { OnCancelOrderErrorHandle(msg); };
  msg_func_map_["OnQueryAsset"] = [this](utils::ItpMsg &msg) { OnQueryAssetHandle(msg); };

  for (auto &iter : msg_func_map_) {
    INFO_LOG("msg_func_map_[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void XtpEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map_.find(msg.msg_name);
  if (iter != msg_func_map_.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msg_name.c_str());
  return;
}

void XtpEvent::OnOrderEventHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order_info = reinterpret_cast<XTPOrderInfo *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(std::to_string(order_info->order_client_id));
  if (content != nullptr) {
    if (order_info->order_status == XTP_ORDER_STATUS_CANCELED || order_info->order_status == XTP_ORDER_STATUS_REJECTED ||
        order_info->order_status == XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING) {
#ifdef BENCH_TEST
      ScopedTimer t("OnRtnOrderHandle");
#endif
      auto &recer_sender = RecerSender::GetInstance();
      if (content->active_cancle_indication) {
        strategy_trader::message message;
        auto *order_cancel_rsp = message.mutable_order_cancel_rsp();
        order_cancel_rsp->set_order_ref(std::to_string(order_info->order_client_id));
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
      INFO_LOG("the order be canceled, orderRef: %d, prid: %s.", order_info->order_client_id, content->prid.c_str());

      order_manage.DelOrder(std::to_string(order_info->order_client_id));
    }
  } else {
    ERROR_LOG("not find order ref: %d", order_info->order_client_id);
  }
}

void XtpEvent::OnTradeEventHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto trade_report = reinterpret_cast<XTPTradeReport *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);

#ifdef BENCH_TEST
  ScopedTimer t("OnRtnTradeHandle");
#endif
  auto content = order_manage.GetOrder(std::to_string(trade_report->order_client_id));
  if (content != nullptr) {
    content->traded_order.price = trade_report->price;
    content->traded_order.volume = trade_report->quantity;
    content->traded_order.direction = trade_report->side;
    content->traded_order.comboffset = trade_report->position_effect + 1;
    content->traded_order.date = trade_report->trade_time;
    content->traded_order.time = trade_report->trade_time;

    strategy_trader::message rsp;
    auto *insert_rsp = rsp.mutable_order_insert_rsp();
    insert_rsp->set_order_ref(std::to_string(trade_report->order_client_id));
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

    content->left_volume -= trade_report->quantity;
    if (content->left_volume == 0) {
      INFO_LOG("the order was finished, ref[%d],identity[%s]", trade_report->order_client_id, content->prid.c_str());
      order_manage.DelOrder(std::to_string(trade_report->order_client_id));
    }
  } else {
    ERROR_LOG("not find order ref: %d", trade_report->order_client_id);
  }
}

void XtpEvent::OnCancelOrderErrorHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto cancel_info = reinterpret_cast<XTPOrderCancelInfo *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(std::to_string(cancel_info->order_xtp_id));
  if (content != nullptr) {
    strategy_trader::message message;
    auto *order_cancel_rsp = message.mutable_order_cancel_rsp();
    order_cancel_rsp->set_order_ref(std::to_string(cancel_info->order_xtp_id));
    order_cancel_rsp->set_result(strategy_trader::Result::failed);
    order_cancel_rsp->set_failedreason("INVALID");

    utils::ItpMsg msg;
    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderCancelRsp." + content->prid;
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(ProxySender).Send(msg);
  } else {
    ERROR_LOG("not find order ref: %ld", cancel_info->order_xtp_id);
  }
}

void XtpEvent::OnQueryAssetHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto asset_rsp = reinterpret_cast<XTPQueryAssetRsp *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *account_rsp = rsp.mutable_account_status_rsp();
  account_rsp->set_result(strategy_trader::Result::success);
  account_rsp->set_user_id(itp_msg.user_id());
  account_rsp->set_session_id(itp_msg.session_id());
  account_rsp->set_balance(asset_rsp->total_asset);
  account_rsp->set_available(asset_rsp->buying_power);

  utils::ItpMsg send_msg;
  rsp.SerializeToString(&send_msg.pb_msg);
  send_msg.session_name = "strategy_trader";
  send_msg.msg_name = "AccountStatusRsp." + std::to_string(itp_msg.request_id());
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).Send(send_msg);
}

bool XtpEvent::SendEmail(const utils::OrderContent &content) {
  char subject_content[30];
  char save_content[200];
  sprintf(subject_content, "%s transaction notice", content.instrument_id.c_str());

  sprintf(save_content,
          "account: %s\ninstrument: %s\norder price: %f\ntransaction price: "
          "%f\ndate: %s\ntime: %s\ndirection: %s\ncomboffset: %s\norder volume: "
          "%d\ntransaction volume: %d",
          content.user_id.c_str(), content.instrument_id.c_str(), content.limit_price, content.traded_order.price,
          content.traded_order.date.c_str(), content.traded_order.time.c_str(), content.traded_order.direction == 1 ? "BUY" : "SELL",
          content.traded_order.comboffset == 1 ? "OPEN" : "CLOSE", content.total_volume, content.traded_order.volume);

  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(EmailSender).Send(subject_content, save_content);

  return true;
}
