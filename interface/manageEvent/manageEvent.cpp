/*
 * manageEvent.cpp
 *
 *  Created on: 2020.11.13
 *      Author: Administrator
 */
#include "trader/interface/manageEvent/manageEvent.h"
#include "common/extern/log/log.h"
#include "common/self/protobuf/manage-trader.pb.h"
#include "trader/domain/traderService.h"
#include "trader/infra/define.h"
#include "trader/infra/recerSender.h"

#include "common/self/semaphorePart.h"
extern GlobalSem globalSem;

bool ManageEvent::init() {
  regMsgFun();

  return true;
}

void ManageEvent::regMsgFun() {
  int cnt = 0;
  msgFuncMap.clear();
  msgFuncMap["AccountStatusReq"] = [this](MsgStruct &msg) { AccountStatusReqHandle(msg); };

  for (auto &iter : msgFuncMap) {
    INFO_LOG("msgFuncMap[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void ManageEvent::handle(MsgStruct &msg) {
  auto iter = msgFuncMap.find(msg.msgName);
  if (iter != msgFuncMap.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msgName [%s]!", msg.msgName.c_str());
  return;
}

void ManageEvent::AccountStatusReqHandle(MsgStruct &msg) {
  manage_trader::message reqMsg;
  reqMsg.ParseFromString(msg.pbMsg);
  auto reqInfo = reqMsg.account_status_req();
  auto identify = reqInfo.process_random_id();
  auto &traderSer = TraderSevice::getInstance();
  if (traderSer.ROLE(Trader).ROLE(CtpTraderApi).getTraderLoginState() != LOGIN_STATE) {
    ERROR_LOG("ctp not login!");
    pubAccountStatusRsp(identify, false, "ctp_logout");
    return;
  }

  traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo.ProcessRandomId = identify;
  auto *traderApi = traderSer.ROLE(Trader).ROLE(CtpTraderApi).traderApi;

  std::string semName = "trader_ReqQryTradingAccount";
  if (traderApi->ReqQryTradingAccount() != 0) {
    ERROR_LOG("req error!");
    pubAccountStatusRsp(identify, false, "req error");
    globalSem.delOrderSem(semName);
    return;
  }

  if (globalSem.waitSemBySemName(semName, 10) != 0) {
    ERROR_LOG("req timeout!");
    pubAccountStatusRsp(identify, false, "req timeout");
    globalSem.delOrderSem(semName);
    return;
  }
  globalSem.delOrderSem(semName);

  if (traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo.rsp_is_null == true) {
    pubAccountStatusRsp(identify, false, "rsp is null");
    return;
  }
  pubAccountStatusRsp(identify, true);
}

void ManageEvent::pubAccountStatusRsp(const std::string &identity, bool result, const std::string &reason) {
  manage_trader::message rsp;
  auto *accountRsp = rsp.mutable_account_status_rsp();
  auto &traderSer = TraderSevice::getInstance();
  accountRsp->set_result(result ? manage_trader::Result::success : manage_trader::Result::failed);

  if (result == true) {
    auto &tmpAccountInfo = traderSer.ROLE(Trader).ROLE(TmpStore).accountInfo;

    accountRsp->set_balance(tmpAccountInfo.Balance);
    accountRsp->set_available(tmpAccountInfo.Available);
  } else {
    accountRsp->set_failedreason(reason);
  }

  std::string strRsp = rsp.SerializeAsString();
  std::string head = "manage_trader.AccountStatusRsp." + identity;
  auto &recerSender = RecerSender::getInstance();
  bool sendRes = recerSender.ROLE(Sender).ROLE(ProxySender).send(head.c_str(), strRsp.c_str());
  if (!sendRes) {
    ERROR_LOG("send OrderInsertRsp error");
    return;
  }
  return;
}