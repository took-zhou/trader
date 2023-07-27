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
#include "trader/infra/recer_sender.h"

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
  if (rsp_info != nullptr && rsp_info->ErrorID != 0) {
    ERROR_LOG("id: %d, msg: %s.", rsp_info->ErrorID, rsp_info->ErrorMsg);
  }

  if (rsp_user_login != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(rsp_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRspUserLogin";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
    session_id = (uint64_t)rsp_user_login->SessionID;
    front_id = rsp_user_login->FrontID;
    user_id = rsp_user_login->UserID;
    front_disconnected = false;
    global_sem.PostSemBySemName(GlobalSem::kLoginLogout);
  } else {
    ERROR_LOG("rsp_user_login is nullptr");
  }
}

void CtpTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *user_logout, CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last) {
  if (rsp_info != nullptr && rsp_info->ErrorID != 0) {
    ERROR_LOG("id: %d, msg: %s.", rsp_info->ErrorID, rsp_info->ErrorMsg);
  }

  if (user_logout != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(rsp_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRspUserLogout";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
    global_sem.PostSemBySemName(GlobalSem::kLoginLogout);
  } else {
    ERROR_LOG("user_logout is nullptr");
  }
}

void CtpTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *settlement_info_confirm,
                                              CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last) {
  auto &global_sem = GlobalSem::GetInstance();
  global_sem.PostSemBySemName(GlobalSem::kLoginLogout);
}

void CtpTraderSpi::OnRtnOrder(CThostFtdcOrderField *order) {
  if (order != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRtnOrder";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("order is nullptr");
  }
}

void CtpTraderSpi::OnRtnTrade(CThostFtdcTradeField *trade) {
  if (trade != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(trade));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRtnTrade";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("trade is nullptr");
  }
}

void CtpTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *trading_account, CThostFtdcRspInfoField *rsp_info, int request_id,
                                          bool is_last) {
  if (rsp_info != nullptr && rsp_info->ErrorID != 0) {
    ERROR_LOG("id: %d, msg: %s.", rsp_info->ErrorID, rsp_info->ErrorMsg);
  }

  if (trading_account != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(trading_account));
    send_msg->set_user_id(user_id);
    send_msg->set_session_id(session_id);
    send_msg->set_is_last(is_last);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRspQryTradingAccount";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("trading_account is nullptr");
  }
}

void CtpTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *instrument, CThostFtdcRspInfoField *rsp_info, int request_id,
                                      bool is_last) {
  if (rsp_info != nullptr && rsp_info->ErrorID != 0) {
    ERROR_LOG("id: %d, msg: %s.", rsp_info->ErrorID, rsp_info->ErrorMsg);
  }

  if (instrument != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(instrument));
    send_msg->set_is_last(is_last);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRspQryInstrument";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("instrument is nullptr");
  }
}

void CtpTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *input_order, CThostFtdcRspInfoField *rsp_info, int request_id,
                                    bool is_last) {
  if (rsp_info != nullptr && rsp_info->ErrorID != 0) {
    ERROR_LOG("id: %d, msg: %s.", rsp_info->ErrorID, rsp_info->ErrorMsg);
  }

  if (input_order != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(input_order));
    send_msg->set_rsp_info(reinterpret_cast<int64_t>(rsp_info));
    send_msg->set_is_last(is_last);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRspOrderInsert";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("input_order_action is nullptr");
  }
}

void CtpTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *input_order_action, CThostFtdcRspInfoField *rsp_info, int request_id,
                                    bool is_last) {
  if (rsp_info != nullptr && rsp_info->ErrorID != 0) {
    ERROR_LOG("id: %d, msg: %s.", rsp_info->ErrorID, rsp_info->ErrorMsg);
  }

  if (input_order_action != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(input_order_action));
    send_msg->set_is_last(is_last);
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRspOrderAction";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("input_order_action is nullptr");
  }
}

void CtpTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *instrument_margin_rate,
                                                CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last) {
  if (rsp_info != nullptr && rsp_info->ErrorID != 0) {
    ERROR_LOG("id: %d, msg: %s.", rsp_info->ErrorID, rsp_info->ErrorMsg);
  }

  if (instrument_margin_rate != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(instrument_margin_rate));
    send_msg->set_user_id(user_id);
    send_msg->set_is_last(is_last);

    // fix bug of exch ins not right
    strcpy(instrument_margin_rate->ExchangeID, req_transaction_cost_exchange.c_str());
    strcpy(instrument_margin_rate->InstrumentID, req_transaction_cost_instrument.c_str());
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRspQryInstrumentMarginRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("instrument_margin_rate is null.");
  }
};

void CtpTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *instrument_commission_rate,
                                                    CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last) {
  if (rsp_info != nullptr && rsp_info->ErrorID != 0) {
    ERROR_LOG("id: %d, msg: %s.", rsp_info->ErrorID, rsp_info->ErrorMsg);
  }

  if (instrument_commission_rate != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(instrument_commission_rate));
    send_msg->set_user_id(user_id);
    send_msg->set_is_last(is_last);

    // fix bug of exch ins not right
    strcpy(instrument_commission_rate->ExchangeID, req_transaction_cost_exchange.c_str());
    strcpy(instrument_commission_rate->InstrumentID, req_transaction_cost_instrument.c_str());
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRspQryInstrumentCommissionRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("instrument_commission_rate is null.");
  }
};

void CtpTraderSpi::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *option_instr_comm_rate, CThostFtdcRspInfoField *rsp_info,
                                               int request_id, bool is_last) {
  if (rsp_info != nullptr && rsp_info->ErrorID != 0) {
    ERROR_LOG("id: %d, msg: %s.", rsp_info->ErrorID, rsp_info->ErrorMsg);
  }

  if (option_instr_comm_rate != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(option_instr_comm_rate));
    send_msg->set_user_id(user_id);
    send_msg->set_is_last(is_last);

    // fix bug of exch ins not right
    strcpy(option_instr_comm_rate->ExchangeID, req_transaction_cost_exchange.c_str());
    strcpy(option_instr_comm_rate->InstrumentID, req_transaction_cost_instrument.c_str());
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "ctp_trader";
    msg.msg_name = "OnRspQryOptionInstrCommRate";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("mm_option_instr_comm_rate is null.");
  }
};
