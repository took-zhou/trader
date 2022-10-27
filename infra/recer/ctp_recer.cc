/*
 * ctpRecer.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include "trader/infra/recer/ctp_recer.h"
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/infra/inner_zmq.h"

void CtpTraderSpi::OnFrontConnected() {
  INFO_LOG("OnFrontConnected():is excuted, re_connect : %d.", re_connect);
  if (re_connect++ == 0) {
    auto &global_sem = GlobalSem::GetInstance();
    global_sem.PostSemBySemName(GlobalSem::kLoginLogout);
  }
}

void CtpTraderSpi::OnFrontDisconnected(int reason) {
  if (reason != 0x1001) {
    ERROR_LOG("front_disconnected, ErrorCode:%#x", reason);
  }
  front_disconnected = true;
}

void CtpTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *rsp_authenticate_field, CThostFtdcRspInfoField *rsp_info,
                                     int request_id, bool is_last) {
  auto &global_sem = GlobalSem::GetInstance();
  global_sem.PostSemBySemName(GlobalSem::kLoginLogout);
}

void CtpTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *rsp_user_login, CThostFtdcRspInfoField *rsp_info, int request_id,
                                  bool is_last) {
  session_id = rsp_user_login->SessionID;
  front_id = rsp_user_login->FrontID;
  user_id = rsp_user_login->UserID;
  auto &global_sem = GlobalSem::GetInstance();
  global_sem.PostSemBySemName(GlobalSem::kLoginLogout);
  front_disconnected = false;
}

void CtpTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *user_logout, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last) {
  auto &global_sem = GlobalSem::GetInstance();
  global_sem.PostSemBySemName(GlobalSem::kLoginLogout);
}

void CtpTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *settlement_info_confirm,
                                              CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last) {
  auto &global_sem = GlobalSem::GetInstance();
  global_sem.PostSemBySemName(GlobalSem::kLoginLogout);
}

void CtpTraderSpi::OnRtnOrder(CThostFtdcOrderField *order) {
  ipc::message req_msg;
  auto send_msg = req_msg.mutable_itp_msg();
  send_msg->set_address(reinterpret_cast<int64_t>(order));
  utils::ItpMsg msg;
  req_msg.SerializeToString(&msg.pb_msg);
  msg.session_name = "ctp_trader";
  msg.msg_name = "OnRtnOrder";

  auto &global_sem = GlobalSem::GetInstance();
  auto &inner_zmq = InnerZmq::GetInstance();
  inner_zmq.PushTask(msg);
  global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
}

void CtpTraderSpi::OnRtnTrade(CThostFtdcTradeField *trade) {
  ipc::message req_msg;
  auto send_msg = req_msg.mutable_itp_msg();
  send_msg->set_address(reinterpret_cast<int64_t>(trade));
  utils::ItpMsg msg;
  req_msg.SerializeToString(&msg.pb_msg);
  msg.session_name = "ctp_trader";
  msg.msg_name = "OnRtnTrade";

  auto &global_sem = GlobalSem::GetInstance();
  auto &inner_zmq = InnerZmq::GetInstance();
  inner_zmq.PushTask(msg);
  global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
}

void CtpTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *trading_account, CThostFtdcRspInfoField *rsp_info, int request_id,
                                          bool is_last) {
  ipc::message req_msg;
  auto send_msg = req_msg.mutable_itp_msg();
  send_msg->set_address(reinterpret_cast<int64_t>(trading_account));
  send_msg->set_user_id(user_id);
  send_msg->set_session_id(session_id);
  send_msg->set_request_id(request_id);
  send_msg->set_is_last(is_last);
  utils::ItpMsg msg;
  req_msg.SerializeToString(&msg.pb_msg);
  msg.session_name = "ctp_trader";
  msg.msg_name = "OnRspQryTradingAccount";

  auto &global_sem = GlobalSem::GetInstance();
  auto &inner_zmq = InnerZmq::GetInstance();
  inner_zmq.PushTask(msg);
  global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
}

void CtpTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *instrument, CThostFtdcRspInfoField *rsp_info, int request_id,
                                      bool is_last) {
  ipc::message req_msg;
  auto send_msg = req_msg.mutable_itp_msg();
  send_msg->set_address(reinterpret_cast<int64_t>(instrument));
  send_msg->set_request_id(request_id);
  send_msg->set_is_last(is_last);
  utils::ItpMsg msg;
  req_msg.SerializeToString(&msg.pb_msg);
  msg.session_name = "ctp_trader";
  msg.msg_name = "OnRspQryInstrument";

  auto &global_sem = GlobalSem::GetInstance();
  auto &inner_zmq = InnerZmq::GetInstance();
  inner_zmq.PushTask(msg);
  global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
}

void CtpTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *input_order_action, CThostFtdcRspInfoField *rsp_info, int request_id,
                                    bool is_last) {
  ipc::message req_msg;
  auto send_msg = req_msg.mutable_itp_msg();
  send_msg->set_address(reinterpret_cast<int64_t>(input_order_action));
  send_msg->set_request_id(request_id);
  send_msg->set_is_last(is_last);
  utils::ItpMsg msg;
  req_msg.SerializeToString(&msg.pb_msg);
  msg.session_name = "ctp_trader";
  msg.msg_name = "OnRspOrderAction";

  auto &global_sem = GlobalSem::GetInstance();
  auto &inner_zmq = InnerZmq::GetInstance();
  inner_zmq.PushTask(msg);
  global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
}

void CtpTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *instrument_margin_rate,
                                                CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last) {
  if (instrument_margin_rate != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(instrument_margin_rate));
    send_msg->set_user_id(user_id);
    send_msg->set_request_id(request_id);
    send_msg->set_is_last(is_last);

    // fix bug of exch ins not right
    strcpy(instrument_margin_rate->ExchangeID, req_transaction_cost_exchange.c_str());
    strcpy(instrument_margin_rate->InstrumentID, req_transaction_cost_instrument.c_str());
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRspQryInstrumentMarginRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &inner_zmq = InnerZmq::GetInstance();
    inner_zmq.PushTask(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("instrument_margin_rate is null.");
  }
};

void CtpTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *instrument_commission_rate,
                                                    CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last) {
  if (instrument_commission_rate != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(instrument_commission_rate));
    send_msg->set_user_id(user_id);
    send_msg->set_request_id(request_id);
    send_msg->set_is_last(is_last);

    // fix bug of exch ins not right
    strcpy(instrument_commission_rate->ExchangeID, req_transaction_cost_exchange.c_str());
    strcpy(instrument_commission_rate->InstrumentID, req_transaction_cost_instrument.c_str());
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRspQryInstrumentCommissionRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &inner_zmq = InnerZmq::GetInstance();
    inner_zmq.PushTask(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("instrument_commission_rate is null.");
  }
};
