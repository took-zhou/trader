/*
 * mtpRecer.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include "trader/infra/recer/mtp_recer.h"
#include "common/extern/log/log.h"
#include "common/self/global_sem.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/utils.h"
#include "trader/infra/recer_sender.h"

void MtpTraderSpi::OnRspUserLogin(const MtpLoginLogoutStruct *login_info) { front_disconnected_ = false; }

void MtpTraderSpi::OnRspUserLogout(const MtpLoginLogoutStruct *logout_info) {}

void MtpTraderSpi::OnRtnTrade(const MtpOrderInfoStruct *trade_info) {
  if (trade_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(trade_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "mtp_trader";
    msg.msg_name = "OnRtnTrade";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("trade info is nullptr");
  }
}

void MtpTraderSpi::OnRtnOrder(const MtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "mtp_trader";
    msg.msg_name = "OnRtnOrder";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order info is nullptr");
  }
}

void MtpTraderSpi::OnRtnOrderInsert(const MtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "mtp_trader";
    msg.msg_name = "OnRtnOrderInsert";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order info is nullptr");
  }
}

void MtpTraderSpi::OnRtnOrderAction(const MtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "mtp_trader";
    msg.msg_name = "OnRtnOrderAction";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order info is nullptr");
  }
}

void MtpTraderSpi::OnRspTradingAccount(const MtpAccountInfo *account_info) {
  if (account_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(account_info));
    send_msg->set_user_id(account_info->user_id);
    send_msg->set_session_id(account_info->session_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "mtp_trader";
    msg.msg_name = "OnRspTradingAccount";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("account info is nullptr");
  }
}

void MtpTraderSpi::OnRspMarginRate(const MtpMarginInfo *margin_info) {
  if (margin_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(margin_info));
    send_msg->set_user_id(margin_info->user_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "mtp_trader";
    msg.msg_name = "OnRspMarginRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("margin info is nullptr");
  }
}

void MtpTraderSpi::OnRspCommissionRate(const MtpCommissionInfo *commission_info) {
  if (commission_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(commission_info));
    send_msg->set_user_id(commission_info->user_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "mtp_trader";
    msg.msg_name = "OnRspCommissionRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("commission info is nullptr");
  }
}

void MtpTraderSpi::OnFrontDisconnected(int reason) { front_disconnected_ = true; }

bool MtpTraderSpi::GetFrontDisconnected(void) { return front_disconnected_; }
