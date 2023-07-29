/*
 * ctpRecer.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include "trader/infra/recer/btp_recer.h"
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/infra/recer_sender.h"
#include "trader/infra/sender/btp_sender.h"

void BtpTraderSpi::OnRspUserLogin(const BtpLoginLogoutStruct *login_info) {}

void BtpTraderSpi::OnRspUserLogout(const BtpLoginLogoutStruct *login_info) {}

void BtpTraderSpi::OnRtnTrade(const BtpOrderInfoStruct *trade_info) {
  if (trade_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(trade_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "btp_trader";
    msg.msg_name = "OnRtnTrade";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("trade_info is nullptr");
  }
}

void BtpTraderSpi::OnRtnOrder(const BtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "btp_trader";
    msg.msg_name = "OnRtnOrder";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("order_info is nullptr");
  }
}

void BtpTraderSpi::OnRtnOrderInsert(const BtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "btp_trader";
    msg.msg_name = "OnRtnOrderInsert";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("order_info is nullptr");
  }
}

void BtpTraderSpi::OnRtnOrderAction(const BtpOrderInfoStruct *order_info) {
  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "btp_trader";
    msg.msg_name = "OnRtnOrderAction";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("order_info is nullptr");
  }
}

void BtpTraderSpi::OnRspTradingAccount(const BtpAccountInfo *account_info) {
  if (account_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(account_info));
    send_msg->set_user_id(account_info->user_id);
    send_msg->set_session_id(account_info->session_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "btp_trader";
    msg.msg_name = "OnRspTradingAccount";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("account_info is nullptr");
  }
}

void BtpTraderSpi::OnRspMarginRate(const BtpMarginInfo *margin_info) {
  if (margin_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(margin_info));
    send_msg->set_user_id(margin_info->user_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "btp_trader";
    msg.msg_name = "OnRspMarginRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("margin_info is nullptr");
  }
}

void BtpTraderSpi::OnRspCommissionRate(const BtpCommissionInfo *commission_info) {
  if (commission_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(commission_info));
    send_msg->set_user_id(commission_info->user_id);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "btp_trader";
    msg.msg_name = "OnRspCommissionRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("commission_info is nullptr");
  }
}
