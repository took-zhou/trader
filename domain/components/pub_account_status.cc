#include "trader/domain/components/pub_account_status.h"
#include <unistd.h>
#include <thread>
#include "common/self/protobuf/strategy-trader.pb.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

PubAccountStatus::PubAccountStatus() {
  ;
  ;
}

void PubAccountStatus::ReqCycle(void) {
  auto &trader_ser = TraderSevice::GetInstance();

  while (1) {
    if (trader_ser.login_state == kLoginState) {
      auto &prid_list = trader_ser.ROLE(ControlPara).GetPridList();
      for (auto &item : prid_list) {
        auto &recer_sender = RecerSender::GetInstance();
        recer_sender.ROLE(Sender).ROLE(ItpSender).ReqAvailableFunds(stoi(item));
        std::this_thread::sleep_for(std::chrono::seconds(10));
      }
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
  }
}
