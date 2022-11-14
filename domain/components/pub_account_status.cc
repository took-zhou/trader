#include "trader/domain/components/pub_account_status.h"
#include <unistd.h>
#include <thread>
#include "common/self/file_util.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

PubAccountStatus::PubAccountStatus() {
  ;
  ;
}

void PubAccountStatus::ReqCycle(void) {
  auto &trader_ser = TraderSevice::GetInstance();
  std::this_thread::sleep_for(std::chrono::seconds(10));

  while (1) {
    auto prid_list = trader_ser.ROLE(ControlPara).GetPridList();
    for (auto &item : prid_list) {
      auto &recer_sender = RecerSender::GetInstance();
      if (trader_ser.login_state == kLoginState) {
        strategy_trader::message send_message;
        auto *accound_status_req = send_message.mutable_account_status_req();
        accound_status_req->set_process_random_id(item);

        utils::ItpMsg itp_msg;
        send_message.SerializeToString(&itp_msg.pb_msg);
        itp_msg.session_name = "strategy_trader";
        itp_msg.msg_name = "AccountStatusReq";
        recer_sender.ROLE(Sender).ROLE(ProxySender).Send(itp_msg);
      }
      std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
  }
}
