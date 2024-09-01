/*
 * otpEvent.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#include "trader/interface/otp_event/otp_event.h"
#include <algorithm>
#include <mutex>
#include <string>
#include "common/extern/log/log.h"
#include "common/extern/otp/inc/oes_api/errors/oes_errors.h"
#include "common/extern/otp/inc/oes_global/oes_base_model.h"
#include "common/extern/otp/inc/oes_global/oes_qry_packets.h"
#include "common/self/file_util.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/protobuf/market-trader.pb.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

OtpEvent::OtpEvent() { RegMsgFun(); }

void OtpEvent::RegMsgFun() {
  msg_func_map_.clear();
  msg_func_map_["OnBusinessReject"] = [this](utils::ItpMsg &msg) { OnBusinessRejectHandle(msg); };
  msg_func_map_["OnOrderReport"] = [this](utils::ItpMsg &msg) { OnOrderReportHandle(msg); };
  msg_func_map_["OnTradeReport"] = [this](utils::ItpMsg &msg) { OnTradeReportHandle(msg); };
  msg_func_map_["OnQueryCashAsset"] = [this](utils::ItpMsg &msg) { OnQueryCashAssetHandle(msg); };
  msg_func_map_["OnQueryCommissionRate"] = [this](utils::ItpMsg &msg) { OnQueryCommissionRateHandle(msg); };
}

void OtpEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map_.find(msg.msg_name);
  if (iter != msg_func_map_.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msg_name [%s]!", msg.msg_name.c_str());
  return;
}

void OtpEvent::OnOrderReportHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order = reinterpret_cast<OesOrdCnfmT *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(std::to_string(order->clOrdId));
  if (content != nullptr) {
    if (order->ordStatus == OES_ORD_STATUS_CANCELED) {
#ifdef BENCH_TEST
      ScopedTimer timer("OnOrderReportHandle");
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
      rsp_info->set_orderprice(order->ordPrice);
      rsp_info->set_ordervolume(order->ordQty);
      rsp.SerializeToString(&msg.pb_msg);
      msg.session_name = "strategy_trader";
      msg.msg_name = "OrderInsertRsp";

      INFO_LOG("the order be canceled, order ref: %lld.", order->clOrdId);
      order_manage.DelOrder(std::to_string(order->clOrdId));
      recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
    }
  } else {
    ERROR_LOG("not find order ref: %lld", order->clOrdId);
  }
}

void OtpEvent::OnTradeReportHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto trade = reinterpret_cast<OesTrdCnfmT *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto &order_lookup = trader_ser.ROLE(OrderLookup);

#ifdef BENCH_TEST
  ScopedTimer t("OnRtnTradeHandle");
#endif
  auto content = order_manage.GetOrder(std::to_string(trade->clSeqNo));
  if (content != nullptr) {
    if (content->comboffset == strategy_trader::CombOffsetType::OPEN) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->user_id, 0, trade->trdQty);
    } else if (content->comboffset == strategy_trader::CombOffsetType::CLOSE_TODAY) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->user_id, 0, -trade->trdQty);
    } else if (content->comboffset == strategy_trader::CombOffsetType::CLOSE_YESTERDAY) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->user_id, -trade->trdQty, 0);
    }

    content->success_volume += trade->trdQty;

    strategy_trader::message rsp;
    auto *insert_rsp = rsp.mutable_order_insert_rsp();
    insert_rsp->set_instrument(content->instrument_id);
    insert_rsp->set_index(content->index);
    insert_rsp->set_result(strategy_trader::Result::success);

    auto *rsp_info = insert_rsp->mutable_info();
    rsp_info->set_orderprice(trade->trdPrice);
    rsp_info->set_ordervolume(trade->trdQty);

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";

    if (content->once_volume == content->success_volume) {
      auto &json_cfg = utils::JsonConfig::GetInstance();
      auto send_email = json_cfg.GetConfig("trader", "SendOrderEmail").get<std::string>();
      if (send_email == "yes") {
        SendEmail(*content);
      }
      INFO_LOG("the order was finished, order ref: %d.", trade->clSeqNo);
      order_manage.DelOrder(std::to_string(trade->clSeqNo));
    }
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
  }
}

void OtpEvent::OnBusinessRejectHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto order_insert_rsp = reinterpret_cast<OesOrdRejectT *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto &order_lookup = trader_ser.ROLE(OrderLookup);
  auto &account_assign = trader_ser.ROLE(AccountAssign);
  auto content = order_manage.GetOrder(std::to_string(order_insert_rsp->origClSeqNo));
  if (content != nullptr) {
    if (content->comboffset == strategy_trader::CombOffsetType::OPEN) {
      account_assign.UpdateOpenBlackList(content->user_id, content->instrument_id, content->index);
    } else if (content->comboffset == strategy_trader::CombOffsetType::CLOSE_YESTERDAY) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->user_id, -content->once_volume, 0);
    } else if (content->comboffset == strategy_trader::CombOffsetType::CLOSE_TODAY) {
      order_lookup.UpdateOpenInterest(content->instrument_id, content->index, content->user_id, 0, -content->once_volume);
    }
    order_manage.DelOrder(std::to_string(order_insert_rsp->origClSeqNo));

    strategy_trader::message message;
    auto *insert_rsp = message.mutable_order_insert_rsp();
    insert_rsp->set_instrument(content->instrument_id);
    insert_rsp->set_index(content->index);
    insert_rsp->set_result(strategy_trader::Result::failed);
    if (order_insert_rsp->ordRejReason == OESERR_ORDER_CASH_NOT_ENOUGH->CODE) {
      insert_rsp->set_reason(strategy_trader::FailedReason::Fund_Shortage_Error);
    } else if (order_insert_rsp->ordRejReason == OESERR_ORDER_HOLDING_NOT_ENOUGH->CODE || OESERR_HOLDING_NOT_FOUND->CODE) {
      insert_rsp->set_reason(strategy_trader::FailedReason::No_Opened_Order);
    } else {
      insert_rsp->set_reason(strategy_trader::FailedReason::Order_Fill_Error);
    }
    auto *rsp_info = insert_rsp->mutable_info();
    rsp_info->set_orderprice(content->limit_price);
    rsp_info->set_ordervolume(content->once_volume);

    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
    INFO_LOG("the order be canceled, orderRef: %d.", order_insert_rsp->origClSeqNo);
  } else {
    ERROR_LOG("not find order ref: %d", order_insert_rsp->origClSeqNo);
  }
}

void OtpEvent::OnQueryCashAssetHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto account = reinterpret_cast<OesCashAssetItemT *>(itp_msg.address());
  auto &trader_ser = TraderService::GetInstance();
  trader_ser.ROLE(AccountAssign)
      .UpdateAccountStatus(account->currentTotalBal, account->currentAvailableBal, itp_msg.session_id(), itp_msg.user_id());
}

void OtpEvent::OnQueryCommissionRateHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto commission_rate_field = reinterpret_cast<OesCommissionRateItemT *>(itp_msg.address());

  strategy_trader::message rsp;
  auto *commission_rate_rsp = rsp.mutable_commission_rate_rsp();

  if (commission_rate_field->mktId == OES_MKT_SH_ASHARE) {
    commission_rate_rsp->set_exchange_id("SHSE");
  } else if (commission_rate_field->mktId == OES_MKT_SZ_ASHARE) {
    commission_rate_rsp->set_exchange_id("SZSE");
  }
  commission_rate_rsp->set_instrument_id(commission_rate_field->securityId);
  commission_rate_rsp->set_user_id(itp_msg.user_id());
  commission_rate_rsp->set_result(strategy_trader::Result::success);
  commission_rate_rsp->set_openratiobymoney(commission_rate_field->feeRate);
  commission_rate_rsp->set_openratiobyvolume(0);
  commission_rate_rsp->set_closeratiobymoney(commission_rate_field->feeRate);
  commission_rate_rsp->set_closeratiobyvolume(0);
  commission_rate_rsp->set_closetodayratiobymoney(commission_rate_field->feeRate);
  commission_rate_rsp->set_closetodayratiobyvolume(0);

  utils::ItpMsg sendmsg;
  rsp.SerializeToString(&sendmsg.pb_msg);
  sendmsg.session_name = "strategy_trader";
  sendmsg.msg_name = "CommissionRateRsp";
  auto &recer_sender = RecerSender::GetInstance();
  recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(sendmsg);
}

bool OtpEvent::SendEmail(const utils::OrderContent &content) {
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
