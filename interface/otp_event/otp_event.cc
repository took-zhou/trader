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
#include "common/extern/otp/inc/oes_api/oes_async_api.h"
#include "common/self/file_util.h"
#include "common/self/profiler.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/protobuf/market-trader.pb.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

OtpEvent::OtpEvent() { RegMsgFun(); }

void OtpEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map_.clear();
  msg_func_map_["OnBusinessReject"] = [this](utils::ItpMsg &msg) { OnBusinessRejectHandle(msg); };
  msg_func_map_["OnOrderReport"] = [this](utils::ItpMsg &msg) { OnOrderReportHandle(msg); };
  msg_func_map_["OnTradeReport"] = [this](utils::ItpMsg &msg) { OnTradeReportHandle(msg); };
  msg_func_map_["OnQueryCashAsset"] = [this](utils::ItpMsg &msg) { OnQueryCashAssetHandle(msg); };
  msg_func_map_["OnQueryCommissionRate"] = [this](utils::ItpMsg &msg) { OnQueryCommissionRateHandle(msg); };

  for (auto &iter : msg_func_map_) {
    INFO_LOG("msg_func_map_[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
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
  auto &trader_ser = TraderSevice::GetInstance();
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
  PZone("OnTradeReportHandle");
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto trade = reinterpret_cast<OesTrdCnfmT *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto &order_lookup = trader_ser.ROLE(OrderLookup);

#ifdef BENCH_TEST
  ScopedTimer t("OnRtnTradeHandle");
#endif
  auto content = order_manage.GetOrder(std::to_string(trade->clSeqNo));
  if (content != nullptr) {
    PZone("SendResult");
    content->traded_order.price = trade->trdPrice;
    content->traded_order.volume = trade->trdQty;
    if (trade->mktId == OES_BS_TYPE_BUY) {
      content->traded_order.direction = 1;
    } else {
      content->traded_order.direction = 2;
    }
    if (trade->mktId == OES_BS_TYPE_BUY) {
      content->traded_order.comboffset = 1;
    } else {
      content->traded_order.comboffset = 2;
    }
    content->traded_order.date = std::to_string(trade->trdDate);
    content->traded_order.time = std::to_string(trade->trdTime);
    content->left_volume -= trade->trdQty;

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
  auto &trader_ser = TraderSevice::GetInstance();
  auto &order_manage = trader_ser.ROLE(OrderManage);
  auto content = order_manage.GetOrder(std::to_string(order_insert_rsp->origClSeqNo));
  if (content != nullptr) {
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

    message.SerializeToString(&msg.pb_msg);
    msg.session_name = "strategy_trader";
    msg.msg_name = "OrderInsertRsp";

    INFO_LOG("the order be canceled, orderRef: %d.", order_insert_rsp->origClSeqNo);
    order_manage.DelOrder(std::to_string(order_insert_rsp->origClSeqNo));
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(Sender).ROLE(DirectSender).SendMsg(msg);
  } else {
    ERROR_LOG("not find order ref: %d", order_insert_rsp->origClSeqNo);
  }
}

void OtpEvent::OnQueryCashAssetHandle(utils::ItpMsg &msg) {
  ipc::message message;
  message.ParseFromString(msg.pb_msg);
  auto &itp_msg = message.itp_msg();

  auto account = reinterpret_cast<OesCashAssetItemT *>(itp_msg.address());
  auto &trader_ser = TraderSevice::GetInstance();
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
