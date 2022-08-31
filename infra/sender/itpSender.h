#ifndef WORKSPACE_TRADER_INFRA_ITPSENDER_H_
#define WORKSPACE_TRADER_INFRA_ITPSENDER_H_

#include "trader/infra/sender/sendApi.h"

struct ItpSender {
 public:
  ItpSender();
  ItpSender(const ItpSender &) = delete;
  ItpSender &operator=(const ItpSender &) = delete;
  static ItpSender &getInstance() {
    static ItpSender instance;
    return instance;
  }

  bool ReqUserLogin();
  bool ReqUserLogout();
  bool InsertOrder(utils::OrderContent &content);
  bool CancelOrder(utils::OrderContent &content);
  bool ReqAvailableFunds(const int requestId);
  bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int requestId);
  bool ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int requestId);
  bool LossConnection();

 private:
  SendApi *sendApi = nullptr;
};

#endif