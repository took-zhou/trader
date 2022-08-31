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
  std::string reqStr;
  reqMsg.SerializeToString(&reqStr);

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.pushTask("xtp_trader.OnOrderEvent", reqStr.c_str());
  globalSem.waitSemBySemName(GlobalSem::apiRecv);
}

void XtpTraderSpi::OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(trade_info));
  std::string reqStr;
  reqMsg.SerializeToString(&reqStr);

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.pushTask("xtp_trader.OnTradeEvent", reqStr.c_str());
  globalSem.waitSemBySemName(GlobalSem::apiRecv);
}

void XtpTraderSpi::OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(cancel_info));
  std::string reqStr;
  reqMsg.SerializeToString(&reqStr);

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.pushTask("xtp_trader.XTPOrderCancelInfo", reqStr.c_str());
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

bool XtpRecer::receMsg(utils::ItpMsg &msg) {
  bool out = true;
  auto &innerZmqBase = InnerZmq::getInstance();

  char *recContent = innerZmqBase.pullTask();
  if (recContent != nullptr) {
    int index = 0;
    int segIndex = 0;
    int length = strlen(recContent) + 1;
    char temp[length];
    for (int i = 0; i < length; i++) {
      temp[index] = recContent[i];
      if (recContent[i] == '.' && segIndex == 0) {
        temp[index] = '\0';
        msg.sessionName = temp;
        index = 0;
        segIndex++;
      } else if (recContent[i] == ' ' && segIndex == 1) {
        temp[index] = '\0';
        msg.msgName = temp;
        index = 0;
        segIndex++;
      } else if (recContent[i] == '\0' && segIndex == 2) {
        msg.pbMsg = temp;
        break;
      } else {
        index++;
      }
    }
  } else {
    out = false;
  }

  return out;
}