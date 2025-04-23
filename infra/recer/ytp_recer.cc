/*
 * ytpRecer.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include "trader/infra/recer/ytp_recer.h"
#include "common/extern/log/log.h"
#include "common/self/global_sem.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/utils.h"
#include "trader/infra/recer_sender.h"

void YtpTraderSpi::OnRspUserLogin(const YtpLoginLogoutStruct *login_info) {
  front_disconnected_ = false;
  auto &global_sem = GlobalSem::GetInstance();
  global_sem.PostSemBySemName(SemName::kLoginLogout);
}

void YtpTraderSpi::OnRspUserLogout(const YtpLoginLogoutStruct *logout_info) {
  auto &global_sem = GlobalSem::GetInstance();
  global_sem.PostSemBySemName(SemName::kLoginLogout);
}

void YtpTraderSpi::OnRtnTrade(const YtpOrderInfoStruct *trade_info) {
  if (trade_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(trade_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ytp_trader";
    msg.msg_name = "OnRtnTrade";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("trade info is nullptr");
  }
}

void YtpTraderSpi::OnRtnOrder(const YtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ytp_trader";
    msg.msg_name = "OnRtnOrder";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order info is nullptr");
  }
}

void YtpTraderSpi::OnRtnOrderInsert(const YtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ytp_trader";
    msg.msg_name = "OnRtnOrderInsert";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order info is nullptr");
  }
}

void YtpTraderSpi::OnRtnOrderAction(const YtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ytp_trader";
    msg.msg_name = "OnRtnOrderAction";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order info is nullptr");
  }
}

void YtpTraderSpi::OnRspTradingAccount(const YtpAccountInfo *account_info) {
  if (account_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(account_info));
    send_msg->set_user_id(account_info->user_id);
    send_msg->set_session_id(account_info->session_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ytp_trader";
    msg.msg_name = "OnRspTradingAccount";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("account info is nullptr");
  }
}

void YtpTraderSpi::OnRspMarginRate(const YtpMarginInfo *margin_info) {
  if (margin_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(margin_info));
    send_msg->set_user_id(margin_info->user_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ytp_trader";
    msg.msg_name = "OnRspMarginRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("margin info is nullptr");
  }
}

void YtpTraderSpi::OnRspCommissionRate(const YtpCommissionInfo *commission_info) {
  if (commission_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(commission_info));
    send_msg->set_user_id(commission_info->user_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ytp_trader";
    msg.msg_name = "OnRspCommissionRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("commission info is nullptr");
  }
}

void YtpTraderSpi::OnFrontDisconnected(int reason) { front_disconnected_ = true; }

bool YtpTraderSpi::GetFrontDisconnected(void) { return front_disconnected_; }
