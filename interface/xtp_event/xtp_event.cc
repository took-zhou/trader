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
#include "common/self/file_util.h"
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
  auto &trader_ser = TraderService::GetInstance();
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
      auto *rsp_info = insert_rsp->mutable_info();
      rsp_info->set_orderprice(order_info->price);
      rsp_info->set_ordervolume(order_info->quantity);
      rsp.SerializeToString(&msg.pb_msg);
      msg.session_name = "strategy_trader";
      msg.msg_name = "OrderInsertRsp";

      INFO_LOG("the order be canceled, orderRef: %d.", order_info->order_client_id);
      order_manage.DelOrder(std::to_string(order_info->order_client_id));
      recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
    }
  }
}

void XtpEvent::OnTradeEventHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto trade_report = reinterpret_cast<XTPTradeReport *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto &order_lookup = trader_ser.ROLE(OrderLookup);

#ifdef BENCH_TEST
  ScopedTimer t("OnRtnTradeHandle");
#endif
  auto content = order_manage.GetOrder(std::to_string(trade_report->order_client_id));
  if (content != nullptr) {
    if (content->comboffset == strategy_trader::CombOffsetType::OPEN) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->user_id, trade_report->price, 0,
                                      trade_report->quantity);
    } else if (content->comboffset == strategy_trader::CombOffsetType::CLOSE_TODAY) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->user_id, trade_report->price, 0,
                                      -trade_report->quantity);
    } else if (content->comboffset == strategy_trader::CombOffsetType::CLOSE_YESTERDAY) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->user_id, trade_report->price,
                                      -trade_report->quantity, 0);
    }

    content->success_volume += trade_report->quantity;

    strategy_trader::message rsp;
    auto *insert_rsp = rsp.mutable_order_insert_rsp();
    insert_rsp->set_instrument(content->instrument_id);
    insert_rsp->set_index(content->index);
    insert_rsp->set_result(strategy_trader::Result::success);

    auto *rsp_info = insert_rsp->mutable_info();
    rsp_info->set_orderprice(trade_report->price);
    rsp_info->set_ordervolume(trade_report->quantity);
    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";

    if (content->total_volume == content->success_volume) {
      auto &json_cfg = utils::JsonConfig::GetInstance();
      auto send_email = json_cfg.GetConfig("trader", "SendOrderEmail").get<std::string>();
      if (send_email == "send") {
        SendEmail(*content);
      }
      INFO_LOG("the order was finished, ref[%d].", trade_report->order_client_id);
      order_manage.DelOrder(std::to_string(trade_report->order_client_id));
    }

    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
  }
}

void XtpEvent::OnCancelOrderErrorHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto cancel_info = reinterpret_cast<XTPOrderCancelInfo *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(std::to_string(cancel_info->order_xtp_id));
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

    INFO_LOG("the order be canceled, orderRef: %s.", std::to_string(cancel_info->order_xtp_id).c_str());
    order_manage.DelOrder(std::to_string(cancel_info->order_xtp_id));
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
  } else {
    ERROR_LOG("not find order ref: %ld", cancel_info->order_xtp_id);
  }
}

void XtpEvent::OnQueryAssetHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto account = reinterpret_cast<XTPQueryAssetRsp *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
  trader_ser.ROLE(AccountAssign).UpdateAccountStatus(account->total_asset, account->buying_power, itp_msg.session_id(), itp_msg.user_id());
}

bool XtpEvent::SendEmail(const utils::OrderContent &content) {
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
