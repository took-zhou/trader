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
#include "common/self/protobuf/manage-trader.pb.h"
#include "common/self/protobuf/market-trader.pb.h"
#include "common/self/protobuf/strategy-trader.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

XtpEvent::XtpEvent() { RegMsgFun(); }

void XtpEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map.clear();
  msg_func_map["OnOrderEvent"] = [this](utils::ItpMsg &msg) { OnOrderEventHandle(msg); };
  msg_func_map["OnTradeEvent"] = [this](utils::ItpMsg &msg) { OnTradeEventHandle(msg); };
  msg_func_map["OnCancelOrderError"] = [this](utils::ItpMsg &msg) { OnCancelOrderErrorHandle(msg); };
  msg_func_map["OnQueryAsset"] = [this](utils::ItpMsg &msg) { OnQueryAssetHandle(msg); };

  for (auto &iter : msg_func_map) {
    INFO_LOG("msg_func_map[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void XtpEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map.find(msg.msgName);
  if (iter != msg_func_map.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msgName.c_str());
  return;
}

void XtpEvent::OnOrderEventHandle(utils::ItpMsg &msg) {
  ipc::message itpMsg;
  itpMsg.ParseFromString(msg.pbMsg);
  auto &itp_msg = itpMsg.itp_msg();

  auto orderInfo = reinterpret_cast<XTPOrderInfo *>(itp_msg.address());
  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);
  auto content = orderManage.get_order(std::to_string(orderInfo->order_client_id));
  if (content != nullptr) {
    if (orderInfo->order_status == XTP_ORDER_STATUS_CANCELED || orderInfo->order_status == XTP_ORDER_STATUS_REJECTED ||
        orderInfo->order_status == XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING) {
#ifdef BENCH_TEST
      ScopedTimer t("OnRtnOrderHandle");
#endif
      auto &recerSender = RecerSender::getInstance();
      if (content->activeCancleIndication) {
        strategy_trader::message rspMsg;
        auto *orderCancelRsp = rspMsg.mutable_order_cancel_rsp();
        orderCancelRsp->set_order_ref(std::to_string(orderInfo->order_client_id));
        orderCancelRsp->set_result(strategy_trader::Result::success);

        utils::ItpMsg msg;
        rspMsg.SerializeToString(&msg.pbMsg);
        msg.sessionName = "strategy_trader";
        msg.msgName = "OrderCancelRsp." + content->prid;
        recerSender.ROLE(Sender).ROLE(ProxySender).Send(msg);
      }
      strategy_trader::message rsp;
      auto *insertRsp = rsp.mutable_order_insert_rsp();
      insertRsp->set_order_ref(content->orderRef);
      insertRsp->set_result(strategy_trader::Result::failed);
      insertRsp->set_reason(strategy_trader::FailedReason::Order_Cancel);
      rsp.SerializeToString(&msg.pbMsg);
      msg.sessionName = "strategy_trader";
      msg.msgName = "OrderInsertRsp." + content->prid;
      recerSender.ROLE(Sender).ROLE(ProxySender).Send(msg);
      INFO_LOG("the order be canceled, orderRef: %d, prid: %s.", orderInfo->order_client_id, content->prid.c_str());

      orderManage.DelOrder(std::to_string(orderInfo->order_client_id));
    }
  }
}

void XtpEvent::OnTradeEventHandle(utils::ItpMsg &msg) {
  ipc::message itpMsg;
  itpMsg.ParseFromString(msg.pbMsg);
  auto &itp_msg = itpMsg.itp_msg();

  auto tradeReport = reinterpret_cast<XTPTradeReport *>(itp_msg.address());
  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);

#ifdef BENCH_TEST
  ScopedTimer t("OnRtnTradeHandle");
#endif
  auto content = orderManage.get_order(std::to_string(tradeReport->order_client_id));
  if (content != nullptr) {
    content->tradedOrder.price = tradeReport->price;
    content->tradedOrder.volume = tradeReport->quantity;
    content->tradedOrder.direction = std::to_string(tradeReport->side);
    content->tradedOrder.date = tradeReport->trade_time;
    content->tradedOrder.time = tradeReport->trade_time;

    strategy_trader::message rsp;
    auto *insertRsp = rsp.mutable_order_insert_rsp();
    insertRsp->set_order_ref(std::to_string(tradeReport->order_client_id));
    insertRsp->set_result(strategy_trader::Result::success);

    auto *succInfo = insertRsp->mutable_info();
    succInfo->set_orderprice(content->tradedOrder.price);
    succInfo->set_ordervolume(content->tradedOrder.volume);

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pbMsg);
    msg.sessionName = "strategy_trader";
    msg.msgName = "OrderInsertRsp." + content->prid;
    auto &recerSender = RecerSender::getInstance();
    recerSender.ROLE(Sender).ROLE(ProxySender).Send(msg);

    SendEmail(*content);

    content->left_volume -= tradeReport->quantity;
    if (content->left_volume == 0) {
      INFO_LOG("the order was finished, ref[%d],identity[%s]", tradeReport->order_client_id, content->prid.c_str());
      orderManage.DelOrder(std::to_string(tradeReport->order_client_id));
    }
  }
}

void XtpEvent::OnCancelOrderErrorHandle(utils::ItpMsg &msg) {
  ipc::message itpMsg;
  itpMsg.ParseFromString(msg.pbMsg);
  auto &itp_msg = itpMsg.itp_msg();

  auto cancelInfo = reinterpret_cast<XTPOrderCancelInfo *>(itp_msg.address());
  auto &traderSer = TraderSevice::getInstance();
  auto &orderManage = traderSer.ROLE(OrderManage);
  auto content = orderManage.get_order(std::to_string(cancelInfo->order_xtp_id));
  if (content != nullptr) {
    strategy_trader::message rspMsg;
    auto *orderCancelRsp = rspMsg.mutable_order_cancel_rsp();
    orderCancelRsp->set_order_ref(std::to_string(cancelInfo->order_xtp_id));
    orderCancelRsp->set_result(strategy_trader::Result::failed);
    orderCancelRsp->set_failedreason("INVALID");

    utils::ItpMsg msg;
    rspMsg.SerializeToString(&msg.pbMsg);
    msg.sessionName = "strategy_trader";
    msg.msgName = "OrderCancelRsp." + content->prid;
    auto &recerSender = RecerSender::getInstance();
    recerSender.ROLE(Sender).ROLE(ProxySender).Send(msg);
  }
}

void XtpEvent::OnQueryAssetHandle(utils::ItpMsg &msg) {
  ipc::message itpMsg;
  itpMsg.ParseFromString(msg.pbMsg);
  auto &itp_msg = itpMsg.itp_msg();

  auto assetRsp = reinterpret_cast<XTPQueryAssetRsp *>(itp_msg.address());
  auto &traderSer = TraderSevice::getInstance();

  if (itp_msg.request_id() == 0) {
    manage_trader::message rsp;
    auto *accountRsp = rsp.mutable_account_status_rsp();
    accountRsp->set_result(manage_trader::Result::success);
    accountRsp->set_user_id(itp_msg.user_id());
    accountRsp->set_session_id(itp_msg.session_id());
    accountRsp->set_balance(assetRsp->total_asset);
    accountRsp->set_available(assetRsp->buying_power);

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pbMsg);
    msg.sessionName = "manage_trader";
    msg.msgName = "AccountStatusRsp.0000000000";
    auto &recerSender = RecerSender::getInstance();
    recerSender.ROLE(Sender).ROLE(ProxySender).Send(msg);
  } else {
    strategy_trader::message rsp;
    auto *accountRsp = rsp.mutable_account_status_rsp();
    accountRsp->set_result(strategy_trader::Result::success);
    accountRsp->set_user_id(itp_msg.user_id());
    accountRsp->set_session_id(itp_msg.session_id());
    accountRsp->set_balance(assetRsp->total_asset);
    accountRsp->set_available(assetRsp->buying_power);

    utils::ItpMsg msg;
    rsp.SerializeToString(&msg.pbMsg);
    msg.sessionName = "strategy_trader";
    msg.msgName = "AccountStatusRsp." + std::to_string(itp_msg.request_id());
    auto &recerSender = RecerSender::getInstance();
    recerSender.ROLE(Sender).ROLE(ProxySender).Send(msg);
  }
}

bool XtpEvent::SendEmail(const utils::OrderContent &content) {
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
  recerSender.ROLE(Sender).ROLE(EmailSender).Send(subjectContent.c_str(), saveContent.c_str());

  return true;
}
