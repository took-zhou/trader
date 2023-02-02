#include "trader/domain/components/account_status.h"
#include <unistd.h>
#include <thread>
#include "common/self/file_util.h"
#include "common/self/protobuf/ipc.pb.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

AccountStatus::AccountStatus() {}

void AccountStatus::InitDatabase() {}

void AccountStatus::UpdateAccountStatus(const std::string &value1, double value2, double value3, uint64_t value4,
                                        const std::string &value5) {
  if (account_info_map.find(value5) != account_info_map.end()) {
    account_info_map[value5]->update_time = value1;
    account_info_map[value5]->balance = value2;
    account_info_map[value5]->available = value3;
    account_info_map[value5]->session_id = value4;
    // INFO_LOG("%s, %lu", value5.c_str(), value4);
  } else {
    account_info_map[value5] = std::make_shared<AccountInfo>(value1, value2, value3, value4, 0);
    INFO_LOG("add account status %s", value5.c_str());
  }
}

void AccountStatus::RemoveAccountStatus(const std::string &value1) {
  if (account_info_map.find(value1) != account_info_map.end()) {
    account_info_map.erase(value1);
  }
}

void AccountStatus::ReqAccountStatus(void) {
  static uint8_t first_login_flag = false;
  auto &trader_ser = TraderSevice::GetInstance();
  auto &recer_sender = RecerSender::GetInstance();
  if (trader_ser.login_state == kLoginState) {
    if (first_login_flag == false) {
      char time_array[64];
      account_info_map.clear();
      auto timenow = trader_ser.ROLE(TraderTimeState).GetTimeNow();
      strftime(time_array, sizeof(time_array), "%Y-%m-%d %H:%M:%S", timenow);
      auto &json_cfg = utils::JsonConfig::GetInstance();
      auto users = json_cfg.GetConfig("trader", "User");
      for (auto &user : users) {
        std::string user_id = json_cfg.GetDeepConfig("users", user, "UserID").get<std::string>();
        UpdateAccountStatus(time_array, 0, 0, 0, user_id);
      }

      first_login_flag = true;
    }
    ipc::message send_message;
    auto *accound_status_req = send_message.mutable_account_status_req();
    accound_status_req->set_indication(ipc::AccountStatusReq::start);

    utils::ItpMsg itp_msg;
    send_message.SerializeToString(&itp_msg.pb_msg);
    itp_msg.session_name = "trader_trader";
    itp_msg.msg_name = "AccountStatusReq";
    // 因为查询接口存在1s1次的限制，所以只能走ProxySender的接口
    recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(itp_msg);
  } else {
    first_login_flag = false;
  }
}

void AccountStatus::SelectAccountStatus(uint64_t *session_id, std::string *order_ref) {
  uint32_t select_count = account_info_map.size();
  static auto pos = account_info_map.begin();
  for (auto &item : account_info_map) {
    INFO_LOG("session id %ld, user id %s", item.second->session_id, item.first.c_str());
  }
  while (select_count-- > 0) {
    if (pos == account_info_map.end()) {
      pos = account_info_map.begin();
    } else {
      if (pos->second->available > 10000) {
        uint32_t ref_index = pos->second->ref_index++;
        *session_id = pos->second->session_id;
        *order_ref = to_string(ref_index + (pos->second->session_id % 1000) * 1000000);
        break;
      }
      pos++;
    }
  }
}

void AccountStatus::RestoreFromSqlite3() {}