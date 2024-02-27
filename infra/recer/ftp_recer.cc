/*
 * ftpRecer.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include "trader/infra/recer/ftp_recer.h"
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/global_sem.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/utils.h"
#include "trader/infra/recer_sender.h"

void FtpTraderSpi::OnRspUserLogin(const FtpLoginLogoutStruct *login_info) {}

void FtpTraderSpi::OnRspUserLogout(const FtpLoginLogoutStruct *logout_info) {}

void FtpTraderSpi::OnRtnTrade(const FtpOrderInfoStruct *trade_info) {
  if (trade_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(trade_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ftp_trader";
    msg.msg_name = "OnRtnTrade";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("trade_info is nullptr");
  }
}

void FtpTraderSpi::OnRtnOrder(const FtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ftp_trader";
    msg.msg_name = "OnRtnOrder";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order_info is nullptr");
  }
}

void FtpTraderSpi::OnRtnOrderInsert(const FtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ftp_trader";
    msg.msg_name = "OnRtnOrderInsert";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order_info is nullptr");
  }
}

void FtpTraderSpi::OnRtnOrderAction(const FtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ftp_trader";
    msg.msg_name = "OnRtnOrderAction";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("order_info is nullptr");
  }
}

void FtpTraderSpi::OnRspTradingAccount(const FtpAccountInfo *account_info) {
  if (account_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(account_info));
    send_msg->set_user_id(account_info->user_id);
    send_msg->set_session_id(account_info->session_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ftp_trader";
    msg.msg_name = "OnRspTradingAccount";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("account_info is nullptr");
  }
}

void FtpTraderSpi::OnRspMarginRate(const FtpMarginInfo *margin_info) {
  if (margin_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(margin_info));
    send_msg->set_user_id(margin_info->user_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ftp_trader";
    msg.msg_name = "OnRspMarginRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("margin_info is nullptr");
  }
}

void FtpTraderSpi::OnRspCommissionRate(const FtpCommissionInfo *commission_info) {
  if (commission_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(commission_info));
    send_msg->set_user_id(commission_info->user_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ftp_trader";
    msg.msg_name = "OnRspCommissionRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(SemName::kApiRecv);
  } else {
    ERROR_LOG("commission_info is nullptr");
  }
}
