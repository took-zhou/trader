/*
 * manageEvent.cpp
 *
 *  Created on: 2020.11.13
 *      Author: Administrator
 */
#include "trader/interface/manage_event/manage_event.h"
#include "common/extern/log/log.h"
#include "common/self/file_util.h"
#include "common/self/protobuf/manage-trader.pb.h"
#include "common/self/semaphore.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

ManageEvent::ManageEvent() { RegMsgFun(); }

void ManageEvent::RegMsgFun() {
  int cnt = 0;
  msg_func_map_.clear();
  msg_func_map_["AccountStatusReq"] = [this](utils::ItpMsg &msg) { AccountStatusReqHandle(msg); };

  for (auto &iter : msg_func_map_) {
    INFO_LOG("msg_func_map_[%d] key is [%s]", cnt, iter.first.c_str());
    cnt++;
  }
}

void ManageEvent::Handle(utils::ItpMsg &msg) {
  auto iter = msg_func_map_.find(msg.msg_name);
  if (iter != msg_func_map_.end()) {
    iter->second(msg);
    return;
  }
  ERROR_LOG("can not find func for msg_name [%s]!", msg.msg_name.c_str());
  return;
}

void ManageEvent::AccountStatusReqHandle(utils::ItpMsg &msg) {
  manage_trader::message rec_message;
  rec_message.ParseFromString(msg.pb_msg);
  auto req_info = rec_message.account_status_req();
  auto process_random_id = req_info.process_random_id();
  auto &trader_ser = TraderSevice::GetInstance();
  if (trader_ser.login_state != kLoginState) {
    ERROR_LOG("itp not login!");
    return;
  }
  auto &recer_sender = RecerSender::GetInstance();

  if (process_random_id == "0000000000") {
    manage_trader::message send_message;
    auto *accound_set_rsp = send_message.mutable_account_set_rsp();
    auto &json_cfg = utils::JsonConfig::GetInstance();
    auto users = json_cfg.GetConfig("trader", "User");
    for (auto &user : users) {
      std::string user_id = json_cfg.GetDeepConfig("users", user, "UserID").get<std::string>();
      accound_set_rsp->add_account(user_id);
    }

    utils::ItpMsg itp_msg;
    send_message.SerializeToString(&itp_msg.pb_msg);

    itp_msg.session_name = "manage_trader";
    itp_msg.msg_name = "AccountSetRsp." + process_random_id;
    recer_sender.ROLE(Sender).ROLE(ProxySender).Send(itp_msg);
  }

  recer_sender.ROLE(Sender).ROLE(ItpSender).ReqAvailableFunds(stoi(process_random_id));
}
