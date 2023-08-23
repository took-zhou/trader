#ifndef WORKSPACE_TRADER_INFRA_ITPSENDER_H_
#define WORKSPACE_TRADER_INFRA_ITPSENDER_H_

#include <memory>
#include "trader/infra/sender/send_api.h"

struct ItpSender {
 public:
  ItpSender();
  ItpSender(const ItpSender &) = delete;
  ItpSender &operator=(const ItpSender &) = delete;
  static ItpSender &GetInstance() {
    static ItpSender instance;
    return instance;
  }

  bool ReqUserLogin();
  bool ReqUserLogout();
  bool InsertOrder(utils::OrderContent &content);
  bool CancelOrder(utils::OrderContent &content);
  bool ReqAvailableFunds();
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch);
  bool LossConnection();

 private:
  std::unique_ptr<SendApi> send_api_;
};

#endif