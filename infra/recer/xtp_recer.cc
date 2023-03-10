/*
 * ctpRecer.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include "trader/infra/recer/xtp_recer.h"
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/infra/recer_sender.h"
#include "trader/infra/sender/xtp_sender.h"

void XtpTraderSpi::OnError(XTPRI *error_info) {
  bool result = ((error_info) && (error_info->error_id != 0));
  if (result) ERROR_LOG("--->>> ErrorID= %d , ErrorMsg= %s.", error_info->error_id, error_info->error_msg);
}

void XtpTraderSpi::OnDisconnected(uint64_t session_id, int reason) {
  ERROR_LOG("front_disconnected, ErrorCode:%#x", reason);
  front_disconnected = true;
}

void XtpTraderSpi::OnRspUserLogin() { front_disconnected = false; }

void XtpTraderSpi::OnRspUserLogout() {}

void XtpTraderSpi::OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id) {
  OnError(error_info);

  if (order_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(order_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "xtp_trader";
    msg.msg_name = "OnOrderEvent";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("order_info is nullptr");
  }
}

void XtpTraderSpi::OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id) {
  if (trade_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(trade_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "xtp_trader";
    msg.msg_name = "OnTradeEvent";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("trade_info is nullptr");
  }
}

void XtpTraderSpi::OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id) {
  OnError(error_info);

  if (cancel_info != nullptr) {
    ipc::message req_msg;
    auto send_msg = req_msg.mutable_itp_msg();
    send_msg->set_address(reinterpret_cast<int64_t>(cancel_info));
    utils::ItpMsg msg;
    req_msg.SerializeToString(&msg.pb_msg);
    msg.session_name = "xtp_trader";
    msg.msg_name = "XTPOrderCancelInfo";

    auto &global_sem = GlobalSem::GetInstance();
    auto &recer_sender = RecerSender::GetInstance();
    recer_sender.ROLE(InnerSender).SendMsg(msg);
    global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
  } else {
    ERROR_LOG("cancel_info is nullptr");
  }
}

void XtpTraderSpi::OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {}

void XtpTraderSpi::OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {}

void XtpTraderSpi::OnQueryPosition(XTPQueryStkPositionRsp *investor_position, XTPRI *error_info, int request_id, bool is_last,
                                   uint64_t session_id) {}

void XtpTraderSpi::OnQueryAsset(XTPQueryAssetRsp *trading_account, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {
  OnError(error_info);

  if (trading_account != nullptr) {
    auto pos = XtpSender::xtp_trader_info_map.find(session_id);
    if (pos != XtpSender::xtp_trader_info_map.end()) {
      ipc::message req_msg;
      auto send_msg = req_msg.mutable_itp_msg();
      send_msg->set_address(reinterpret_cast<int64_t>(trading_account));
      send_msg->set_user_id(pos->second.user_id);
      send_msg->set_session_id(session_id);
      send_msg->set_is_last(is_last);
      utils::ItpMsg msg;
      req_msg.SerializeToString(&msg.pb_msg);
      msg.session_name = "xtp_trader";
      msg.msg_name = "OnQueryAsset";

      auto &global_sem = GlobalSem::GetInstance();
      auto &recer_sender = RecerSender::GetInstance();
      recer_sender.ROLE(InnerSender).SendMsg(msg);
      global_sem.WaitSemBySemName(GlobalSem::kApiRecv);
    }
  } else {
    ERROR_LOG("trading_account is nullptr");
  }
}

void XtpTraderSpi::OnQueryStructuredFund(XTPStructuredFundInfo *fund_info, XTPRI *error_info, int request_id, bool is_last,
                                         uint64_t session_id) {}

void XtpTraderSpi::OnQueryFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, int request_id, bool is_last,
                                       uint64_t session_id) {}

void XtpTraderSpi::OnFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, uint64_t session_id) {}

void XtpTraderSpi::OnQueryETF(XTPQueryETFBaseRsp *etf_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {}

void XtpTraderSpi::OnQueryETFBasket(XTPQueryETFComponentRsp *etf_component_info, XTPRI *error_info, int request_id, bool is_last,
                                    uint64_t session_id) {}

void XtpTraderSpi::OnQueryIPOInfoList(XTPQueryIPOTickerRsp *ipo_info, XTPRI *error_info, int request_id, bool is_last,
                                      uint64_t session_id) {}

void XtpTraderSpi::OnQueryIPOQuotaInfo(XTPQueryIPOQuotaRsp *quota_info, XTPRI *error_info, int request_id, bool is_last,
                                       uint64_t session_id) {}

void XtpTraderSpi::OnQueryOptionAuctionInfo(XTPQueryOptionAuctionInfoRsp *option_info, XTPRI *error_info, int request_id, bool is_last,
                                            uint64_t session_id) {}

void XtpTraderSpi::OnCreditCashRepay(XTPCrdCashRepayRsp *cash_repay_info, XTPRI *error_info, uint64_t session_id) {}

void XtpTraderSpi::OnQueryCreditCashRepayInfo(XTPCrdCashRepayInfo *cash_repay_info, XTPRI *error_info, int request_id, bool is_last,
                                              uint64_t session_id) {}

void XtpTraderSpi::OnQueryCreditFundInfo(XTPCrdFundInfo *fund_info, XTPRI *error_info, int request_id, uint64_t session_id) {}

void XtpTraderSpi::OnQueryCreditDebtInfo(XTPCrdDebtInfo *debt_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {}

void XtpTraderSpi::OnQueryCreditTickerDebtInfo(XTPCrdDebtStockInfo *debt_info, XTPRI *error_info, int request_id, bool is_last,
                                               uint64_t session_id) {}

void XtpTraderSpi::OnQueryCreditAssetDebtInfo(double remain_amount, XTPRI *error_info, int request_id, uint64_t session_id) {}

void XtpTraderSpi::OnQueryCreditTickerAssignInfo(XTPClientQueryCrdPositionStkInfo *assign_info, XTPRI *error_info, int request_id,
                                                 bool is_last, uint64_t session_id) {}

void XtpTraderSpi::OnQueryCreditExcessStock(XTPClientQueryCrdSurplusStkRspInfo *stock_info, XTPRI *error_info, int request_id,
                                            uint64_t session_id) {}
