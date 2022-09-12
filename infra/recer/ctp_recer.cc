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
  static int reConnect = 0;
  INFO_LOG("OnFrontConnected():is excuted...");
  if (reConnect++ == 0) {
    auto &globalSem = GlobalSem::getInstance();
    globalSem.PostSemBySemName(GlobalSem::kLoginLogout);
  }
}

void CtpTraderSpi::OnFrontDisconnected(int nReason) {
  ERROR_LOG("front_disconnected, ErrorCode:%#x", nReason);
  front_disconnected = true;
}

void CtpTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo,
                                     int nRequestID, bool bIsLast) {
  auto &globalSem = GlobalSem::getInstance();
  globalSem.PostSemBySemName(GlobalSem::kLoginLogout);
}

void CtpTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                                  bool bIsLast) {
  session_id = pRspUserLogin->SessionID;
  front_id = pRspUserLogin->FrontID;
  auto &globalSem = GlobalSem::getInstance();
  globalSem.PostSemBySemName(GlobalSem::kLoginLogout);
  front_disconnected = false;
}

void CtpTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  auto &globalSem = GlobalSem::getInstance();
  globalSem.PostSemBySemName(GlobalSem::kLoginLogout);
}

void CtpTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
                                              CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  auto &globalSem = GlobalSem::getInstance();
  globalSem.PostSemBySemName(GlobalSem::kLoginLogout);
}

void CtpTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(pOrder));
  utils::ItpMsg msg;
  reqMsg.SerializeToString(&msg.pbMsg);
  msg.sessionName = "ctp_trader";
  msg.msgName = "OnRtnOrder";

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.PushTask(msg);
  globalSem.WaitSemBySemName(GlobalSem::kApiRecv);
}

void CtpTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(pTrade));
  utils::ItpMsg msg;
  reqMsg.SerializeToString(&msg.pbMsg);
  msg.sessionName = "ctp_trader";
  msg.msgName = "OnRtnTrade";

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.PushTask(msg);
  globalSem.WaitSemBySemName(GlobalSem::kApiRecv);
}

void CtpTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                                          bool bIsLast) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(pTradingAccount));
  sendMsg->set_user_id(pTradingAccount->AccountID);
  sendMsg->set_session_id(session_id);
  sendMsg->set_request_id(nRequestID);
  sendMsg->set_is_last(bIsLast);
  utils::ItpMsg msg;
  reqMsg.SerializeToString(&msg.pbMsg);
  msg.sessionName = "ctp_trader";
  msg.msgName = "OnRspQryTradingAccount";

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.PushTask(msg);
  globalSem.WaitSemBySemName(GlobalSem::kApiRecv);
}

void CtpTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                                      bool bIsLast) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(pInstrument));
  sendMsg->set_request_id(nRequestID);
  sendMsg->set_is_last(bIsLast);
  utils::ItpMsg msg;
  reqMsg.SerializeToString(&msg.pbMsg);
  msg.sessionName = "ctp_trader";
  msg.msgName = "OnRspQryInstrument";

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.PushTask(msg);
  globalSem.WaitSemBySemName(GlobalSem::kApiRecv);
}

void CtpTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                                    bool bIsLast) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(pInputOrderAction));
  sendMsg->set_request_id(nRequestID);
  sendMsg->set_is_last(bIsLast);
  utils::ItpMsg msg;
  reqMsg.SerializeToString(&msg.pbMsg);
  msg.sessionName = "ctp_trader";
  msg.msgName = "OnRspOrderAction";

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.PushTask(msg);
  globalSem.WaitSemBySemName(GlobalSem::kApiRecv);
}

void CtpTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate,
                                                CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(pInstrumentMarginRate));
  sendMsg->set_request_id(nRequestID);
  sendMsg->set_is_last(bIsLast);
  utils::ItpMsg msg;
  reqMsg.SerializeToString(&msg.pbMsg);
  msg.sessionName = "ctp_trader";
  msg.msgName = "OnRspQryInstrumentMarginRate";

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.PushTask(msg);
  globalSem.WaitSemBySemName(GlobalSem::kApiRecv);
};

void CtpTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate,
                                                    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  ipc::message reqMsg;
  auto sendMsg = reqMsg.mutable_itp_msg();
  sendMsg->set_address(reinterpret_cast<int64_t>(pInstrumentCommissionRate));
  sendMsg->set_request_id(nRequestID);
  sendMsg->set_is_last(bIsLast);
  utils::ItpMsg msg;
  reqMsg.SerializeToString(&msg.pbMsg);
  msg.sessionName = "ctp_trader";
  msg.msgName = "OnRspQryInstrumentCommissionRate";

  auto &globalSem = GlobalSem::getInstance();
  auto &innerZmq = InnerZmq::getInstance();
  innerZmq.PushTask(msg);
  globalSem.WaitSemBySemName(GlobalSem::kApiRecv);
};
