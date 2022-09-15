#ifndef WORKSPACE_TRADER_INFRA_ITPSENDER_H_
#define WORKSPACE_TRADER_INFRA_ITPSENDER_H_

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
  bool ReqAvailableFunds(const int request_id);
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int request_id);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int request_id);
  bool LossConnection();

 private:
  SendApi *send_api_ = nullptr;
};

#endif