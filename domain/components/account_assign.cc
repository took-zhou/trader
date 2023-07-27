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
  if (account_info_map.find(value4) != account_info_map.end()) {
    account_info_map[value4]->balance = value1;
    account_info_map[value4]->available = value2;
    if (account_info_map[value4]->session_id != value3) {
      INFO_LOG("session id change %ld->%ld", account_info_map[value4]->session_id, value3);
      account_info_map[value4]->session_id = value3;
      account_info_map[value4]->order_ref = (uint32_t)((value3 % 1000) * 1000000);
    }
    // INFO_LOG("user id: %s, session id: %lu, available: %f", value5.c_str(), value4, value3);
  } else {
    auto order_ref_base = (uint32_t)((value3 % 1000) * 1000000);
    account_info_map[value4] = std::make_shared<AccountInfo>(value1, value2, value3, order_ref_base);
    INFO_LOG("add account status %s", value4.c_str());
  }
}

void AccountAssign::UpdateOpenBlackList(const std::string &value, const std::string &ins, const std::string &index) {
  std::string temp_key;
  temp_key += ins;
  temp_key += ".";
  temp_key += index;
  if (account_info_map.find(value) != account_info_map.end()) {
    account_info_map[value]->open_blacklist.insert(temp_key);
  }
}

void AccountAssign::RemoveAccountStatus() { account_info_map.clear(); }

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

void AccountAssign::RestoreFromSqlite3() {}