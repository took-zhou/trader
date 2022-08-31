#ifndef __TRADERSENDAPI__
#define __TRADERSENDAPI__
#include <vector>
#include "common/self/utils.h"

struct SendApi {
 public:
  virtual bool ReqUserLogin() = 0;
  virtual bool ReqUserLogout() = 0;
  virtual bool InsertOrder(utils::OrderContent &content) = 0;
  virtual bool CancelOrder(utils::OrderContent &content) = 0;
  virtual bool ReqAvailableFunds(const int requestId) = 0;
  virtual bool ReqInstrumentInfo(const utils::InstrumtntID &ins_exch, const int requestId) = 0;
  virtual bool ReqTransactionCost(const utils::InstrumtntID &ins_exch, const int requestId) = 0;
  virtual bool LossConnection() = 0;
};

#endif