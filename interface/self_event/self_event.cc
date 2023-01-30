/*
 * strategyEvent.cpp
 */
#include <fstream>
#include <iomanip>
#include <iostream>

#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/profiler.h"
#include "common/self/protobuf/ipc.pb.h"
#include "common/self/semaphore.h"
#include "common/self/utils.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"
#include "trader/interface/self_event/self_event.h"

SelfEvent::SelfEvent() { RegMsgFun(); }

void SelfEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map_.find(msg.msg_name);
  if (iter != msg_func_map_.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msg_name [%s]!", msg.msg_name.c_str());
  return;
}

void SelfEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map_.clear();
  msg_func_map_["AccountStatusReq"] = [this](utils::ItpMsg &msg) { AccountStatusReqHandle(msg); };
  msg_func_map_["SendEmail"] = [this](utils::ItpMsg &msg) { SendEmailHandle(msg); };

  for (auto &iter : msg_func_map_) {
    INFO_LOG("msg_func_map_[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
  return;
}

void SelfEvent::AccountStatusReqHandle(utils::ItpMsg &msg) {
  auto &recer_sender = RecerSender::GetInstance();

  ipc::message recv_message;
  recv_message.ParseFromString(msg.pb_msg);

  if (recv_message.account_status_req().indication() == ipc::AccountStatusReq::start) {
    recer_sender.ROLE(Sender).ROLE(ItpSender).ReqAvailableFunds(0);
  }
}

void SelfEvent::SendEmailHandle(utils::ItpMsg &msg) {
  auto &recer_sender = RecerSender::GetInstance();
  ipc::message recv_message;
  recv_message.ParseFromString(msg.pb_msg);
  auto &head = recv_message.send_email().head();
  auto &body = recv_message.send_email().body();
  recer_sender.ROLE(Sender).ROLE(EmailSender).Send(head.c_str(), body.c_str());
}