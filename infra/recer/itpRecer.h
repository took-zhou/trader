#ifndef WORKSPACE_TRADER_INFRA_ITPRECER_H_
#define WORKSPACE_TRADER_INFRA_ITPRECER_H_

#include "trader/infra/recer/recerSpi.h"

struct ItpRecer {
 public:
  ItpRecer();
  ItpRecer(const ItpRecer &) = delete;
  ItpRecer &operator=(const ItpRecer &) = delete;
  static ItpRecer &getInstance() {
    static ItpRecer instance;
    return instance;
  }

  bool receMsg(utils::ItpMsg &msg);

 private:
  RecerSpi *recerSpi = nullptr;
};

#endif