#include "trader/domain/components/account_assign.h"
#include <unistd.h>
#include <thread>
#include "common/self/file_util.h"
#include "common/self/protobuf/ipc.pb.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

AccountAssign::AccountAssign() {}

void AccountAssign::InitDatabase() {}

void AccountAssign::UpdateAccountStatus(double value1, double value2, uint64_t value3, const std::string &value4) {
  if (account_info_map_.find(value3) != account_info_map_.end()) {
    account_info_map_[value3]->balance = value1;
    account_info_map_[value3]->available = value2;
    account_info_map_[value3]->user_id = value4;
    // INFO_LOG("user id: %s, session id: %lu, available: %f", value5.c_str(), value4, value3);
  } else {
    auto order_ref_base = (uint32_t)((value3 % 1000) * 1000000);
    account_info_map_[value3] = std::make_shared<AccountInfo>(value1, value2, value4, order_ref_base);
    INFO_LOG("add account status %s", value4.c_str());
  }
}

void AccountAssign::RemoveAccountStatus() { account_info_map_.clear(); }

void AccountAssign::ReqAccountStatus(void) {
  auto &trader_ser = TraderSevice::GetInstance();
  auto &recer_sender = RecerSender::GetInstance();
  if (trader_ser.login_state == kLoginState) {
    ipc::message send_message;
    auto *accound_status_req = send_message.mutable_account_status_req();
    accound_status_req->set_indication(ipc::AccountStatusReq::start);

    utils::ItpMsg itp_msg;
    send_message.SerializeToString(&itp_msg.pb_msg);
    itp_msg.session_name = "trader_trader";
    itp_msg.msg_name = "AccountStatusReq";
    // 因为查询接口存在1s1次的限制，所以只能走ProxySender的接口
    recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(itp_msg);
  }
}

void AccountAssign::BuildOrderContent(std::shared_ptr<utils::OrderContent> &content) {
  if (content->comboffset == 1) {
    uint32_t find_count = 0;
    for (static auto pos = account_info_map_.begin();; pos++) {
      if (pos == account_info_map_.end()) {
        pos = account_info_map_.begin();
      }
      if (find_count++ > account_info_map_.size()) {
        ERROR_LOG("can not find account.");
        pos++;
        break;
      }
      if (pos->second->available >= minimum_account_available_) {
        content->session_id = pos->first;
        content->order_ref = to_string(pos->second->order_ref++);
        pos++;
        break;
      }
    }
  } else {
    auto &trader_ser = TraderSevice::GetInstance();
    std::string temp_key;
    temp_key += content->prid;
    temp_key += ".";
    temp_key += content->instrument_id;
    temp_key += ".";
    temp_key += content->index;
    auto pos = trader_ser.ROLE(OrderLookup).GetOrderPara(temp_key);
    if (pos != nullptr) {
      content->session_id = pos->session_id;
      content->order_ref = to_string(account_info_map_[pos->session_id]->order_ref++);
    } else {
      INFO_LOG("can not find session id. %s", temp_key.c_str());
    }
  }
}

void AccountAssign::RestoreFromSqlite3() {}