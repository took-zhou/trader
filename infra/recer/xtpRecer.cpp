/*
 * ctpRecer.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include "trader/infra/recer/xtpRecer.h"
#include "common/extern/log/log.h"
#include "common/self/fileUtil.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/semaphorePart.h"
#include "common/self/utils.h"
#include "trader/infra/innerZmq.h"

void XtpTraderSpi::OnError(XTPRI *error_info) { ; }

void XtpTraderSpi::OnDisconnected(uint64_t session_id, int reason) {
  ERROR_LOG("OnFrontDisconnected, ErrorCode:%#x", reason);
  frontDisconnected = true;
}

void XtpTraderSpi::OnRspUserLogin() { frontDisconnected = false; }

void XtpTraderSpi::OnRspUserLogout() {}

void XtpTraderSpi::OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(order_info));
  utils::ItpMsg msg;
  reqMsg.SerializeToString(&msg.pbMsg);
  msg.sessionName = "xtp_trader";
  msg.msgName = "OnOrderEvent";

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.pushTask(msg);
  globalSem.waitSemBySemName(GlobalSem::apiRecv);
}

void XtpTraderSpi::OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(trade_info));
  utils::ItpMsg msg;
  reqMsg.SerializeToString(&msg.pbMsg);
  msg.sessionName = "xtp_trader";
  msg.msgName = "OnTradeEvent";

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.pushTask(msg);
  globalSem.waitSemBySemName(GlobalSem::apiRecv);
}

void XtpTraderSpi::OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(cancel_info));
  utils::ItpMsg msg;
  reqMsg.SerializeToString(&msg.pbMsg);
  msg.sessionName = "xtp_trader";
  msg.msgName = "XTPOrderCancelInfo";

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.pushTask(msg);
  globalSem.waitSemBySemName(GlobalSem::apiRecv);
}

void XtpTraderSpi::OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {}

void XtpTraderSpi::OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {}

void XtpTraderSpi::OnQueryPosition(XTPQueryStkPositionRsp *investor_position, XTPRI *error_info, int request_id, bool is_last,
                                   uint64_t session_id) {}

void XtpTraderSpi::OnQueryAsset(XTPQueryAssetRsp *trading_account, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {}
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
