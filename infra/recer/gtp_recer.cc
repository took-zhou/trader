/*
 * gtpRecer.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include "trader/infra/recer/gtp_recer.h"
#include "common/extern/log/log.h"
#include "common/self/global_sem.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/utils.h"
#include "trader/infra/recer_sender.h"

void GtpTraderSpi::OnRspUserLogin(const GtpLoginLogoutStruct *login_info) {
  front_disconnected_ = false;
  auto &global_sem = GlobalSem::GetInstance();
  global_sem.PostSemBySemName(SemName::kLoginLogout);
}

void GtpTraderSpi::OnRspUserLogout(const GtpLoginLogoutStruct *logout_info) {
  auto &global_sem = GlobalSem::GetInstance();
  global_sem.PostSemBySemName(SemName::kLoginLogout);
}

void GtpTraderSpi::OnRtnTrade(const GtpOrderInfoStruct *trade_info) {
  if (trade_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(trade_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "gtp_trader";
    msg.msg_name = "OnRtnTrade";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("trade info is nullptr");
  }
}

void GtpTraderSpi::OnRtnOrder(const GtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "gtp_trader";
    msg.msg_name = "OnRtnOrder";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order info is nullptr");
  }
}

void GtpTraderSpi::OnRtnOrderInsert(const GtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "gtp_trader";
    msg.msg_name = "OnRtnOrderInsert";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order info is nullptr");
  }
}

void GtpTraderSpi::OnRtnOrderAction(const GtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "gtp_trader";
    msg.msg_name = "OnRtnOrderAction";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order info is nullptr");
  }
}

void GtpTraderSpi::OnRspTradingAccount(const GtpAccountInfo *account_info) {
  if (account_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(account_info));
    send_msg->set_user_id(account_info->user_id);
    send_msg->set_session_id(account_info->session_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "gtp_trader";
    msg.msg_name = "OnRspTradingAccount";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("account info is nullptr");
  }
}

void GtpTraderSpi::OnRspMarginRate(const GtpMarginInfo *margin_info) {
  if (margin_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(margin_info));
    send_msg->set_user_id(margin_info->user_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "gtp_trader";
    msg.msg_name = "OnRspMarginRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("margin info is nullptr");
  }
}

void GtpTraderSpi::OnRspCommissionRate(const GtpCommissionInfo *commission_info) {
  if (commission_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(commission_info));
    send_msg->set_user_id(commission_info->user_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "gtp_trader";
    msg.msg_name = "OnRspCommissionRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("commission info is nullptr");
  }
}

void GtpTraderSpi::OnFrontDisconnected(int reason) { front_disconnected_ = true; }

bool GtpTraderSpi::GetFrontDisconnected(void) { return front_disconnected_; }
