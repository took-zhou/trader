/*
 * ftpEvent.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/interface/ftp_event/ftp_event.h"
#include <algorithm>
#include <mutex>
#include "common/extern/ftp/inc/ftp_trader_struct.h"
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/protobuf/market-trader.pb.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

FtpEvent::FtpEvent() { RegMsgFun(); }

void FtpEvent::RegMsgFun() {
  msg_func_map_.clear();
  msg_func_map_["OnRtnOrder"] = [this](utils::ItpMsg &msg) { OnRtnOrderHandle(msg); };
  msg_func_map_["OnRtnTrade"] = [this](utils::ItpMsg &msg) { OnRtnTradeHandle(msg); };
  msg_func_map_["OnRtnOrderInsert"] = [this](utils::ItpMsg &msg) { OnRtnOrderInsertHandle(msg); };
  msg_func_map_["OnRtnOrderAction"] = [this](utils::ItpMsg &msg) { OnRtnOrderActionHandle(msg); };
  msg_func_map_["OnRspTradingAccount"] = [this](utils::ItpMsg &msg) { OnRspTradingAccountHandle(msg); };
  msg_func_map_["OnRspMarginRate"] = [this](utils::ItpMsg &msg) { OnRspMarginRateHandle(msg); };
  msg_func_map_["OnRspCommissionRate"] = [this](utils::ItpMsg &msg) { OnRspCommissionRateHandle(msg); };
}

void FtpEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map_.find(msg.msg_name);
  if (iter != msg_func_map_.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msg name [%s]!", msg.msg_name.c_str());
  return;
}

void FtpEvent::OnRtnOrderHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order_info = reinterpret_cast<FtpOrderInfoStruct *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(std::to_string(order_info->order_ref));
  if (content != nullptr) {
    if (order_info->order_status == FtpOrderInfoStruct::kCanceled) {
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
      rsp_info->set_ordervolume(order_info->volume);
      rsp.SerializeToString(&msg.pb_msg);
      msg.session_name = "strategy_trader";
      msg.msg_name = "OrderInsertRsp";

      order_manage.DelOrder(std::to_string(order_info->order_ref));
      recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
      INFO_LOG("the order be canceled, order ref: %d, volume: %d.", order_info->order_ref, order_info->volume);
    }
  }
}

void FtpEvent::OnRtnTradeHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto trade_report = reinterpret_cast<FtpOrderInfoStruct *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto &order_lookup = trader_ser.ROLE(OrderLookup);

#ifdef BENCH_TEST
  ScopedTimer timer("OnRtnTradeHandle");
#endif
  auto content = order_manage.GetOrder(std::to_string(trade_report->order_ref));
  if (content != nullptr) {
    if (content->comboffset == strategy_trader::CombOffsetType::OPEN) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->group_id, content->user_id, 0, trade_report->volume);
    } else if (content->comboffset == strategy_trader::CombOffsetType::CLOSE_TODAY) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->group_id, content->user_id, 0,
                                      -trade_report->volume);
    } else if (content->comboffset == strategy_trader::CombOffsetType::CLOSE_YESTERDAY) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->group_id, content->user_id, -trade_report->volume,
                                      0);
    }

    content->success_volume += trade_report->volume;

    strategy_trader::message rsp;
    auto *insert_rsp = rsp.mutable_order_insert_rsp();
    insert_rsp->set_result(strategy_trader::Result::success);
    insert_rsp->set_instrument(content->instrument_id);
    insert_rsp->set_index(content->index);

    auto *rsp_info = insert_rsp->mutable_info();
    rsp_info->set_orderprice(trade_report->price);
    rsp_info->set_ordervolume(trade_report->volume);
    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";

    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
    INFO_LOG("the order be traded, order ref: %d, volume: %d.", trade_report->order_ref, trade_report->volume);

    if (content->once_volume == content->success_volume) {
      auto &json_cfg = utils::JsonConfig::GetInstance();
      auto send_email = json_cfg.GetConfig("trader", "SendOrderEmail").get<std::string>();
      if (send_email == "yes") {
        SendEmail(*content);
      }
      INFO_LOG("the order was finished, ref[%d].", trade_report->order_ref);
      order_manage.DelOrder(std::to_string(trade_report->order_ref));
    }
  }
}

void FtpEvent::OnRtnOrderInsertHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order_insert = reinterpret_cast<FtpOrderInfoStruct *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto &order_lookup = trader_ser.ROLE(OrderLookup);
  auto &account_assign = trader_ser.ROLE(AccountAssign);
  auto content = order_manage.GetOrder(std::to_string(order_insert->order_ref));
  if (content != nullptr) {
    if (content->comboffset == strategy_trader::CombOffsetType::OPEN) {
      account_assign.UpdateOpenBlackList(content->user_id, content->instrument_id, content->index);
    } else if (content->comboffset == strategy_trader::CombOffsetType::CLOSE_YESTERDAY) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->group_id, content->user_id, -content->once_volume,
                                      0);
    } else if (content->comboffset == strategy_trader::CombOffsetType::CLOSE_TODAY) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->group_id, content->user_id, 0,
                                      -content->once_volume);
    }

    strategy_trader::message message;
    auto *insert_rsp = message.mutable_order_insert_rsp();
    insert_rsp->set_instrument(content->instrument_id);
    insert_rsp->set_index(content->index);
    insert_rsp->set_result(strategy_trader::Result::failed);
    if (order_insert->order_status == FtpOrderInfoStruct::kRejected) {
      insert_rsp->set_reason(strategy_trader::FailedReason::Fund_Shortage_Error);
    } else if (order_insert->order_status == FtpOrderInfoStruct::kNoOpened) {
      insert_rsp->set_reason(strategy_trader::FailedReason::No_Opened_Order);
    }
    auto *rsp_info = insert_rsp->mutable_info();
    rsp_info->set_orderprice(content->limit_price);
    rsp_info->set_ordervolume(content->once_volume);

    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
    order_manage.DelOrder(std::to_string(order_insert->order_ref));
    INFO_LOG("the order be canceled, order ref: %d, volume: %d.", order_insert->order_ref, content->once_volume);
  } else {
    ERROR_LOG("not find order ref: %d", order_insert->order_ref);
  }
}

void FtpEvent::OnRtnOrderActionHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order_action_rsp = reinterpret_cast<FtpOrderInfoStruct *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
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

void FtpEvent::OnRspTradingAccountHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto account = reinterpret_cast<FtpAccountInfo *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
  trader_ser.ROLE(AccountAssign).UpdateAccountStatus(account->balance, account->available, itp_msg.session_id(), itp_msg.user_id());
}

void FtpEvent::OnRspMarginRateHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto margin_info = reinterpret_cast<FtpMarginInfo *>(itp_msg.address());

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

void FtpEvent::OnRspCommissionRateHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto commission_info = reinterpret_cast<FtpCommissionInfo *>(itp_msg.address());

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

bool FtpEvent::SendEmail(const utils::OrderContent &content) {
  char subject_content[30];
  char save_content[180];
  sprintf(subject_content, "%s transaction notice", content.instrument_id.c_str());

  sprintf(save_content, "account: %s\ninstrument: %s\norder price: %.15g\ndirection: %s\ncomboffset: %s\norder volume: %d",
          content.user_id.c_str(), content.instrument_id.c_str(), content.limit_price, content.direction == 1 ? "BUY" : "SELL",
          content.comboffset == 1 ? "OPEN" : "CLOSE", content.once_volume);

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
