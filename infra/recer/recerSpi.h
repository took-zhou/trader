#ifndef __TRADERRECERSPI__
#define __TRADERRECERSPI__
#include <vector>
#include "common/self/utils.h"

struct RecerSpi {
 public:
  virtual bool receMsg(utils::ItpMsg &msg) = 0;
};

#endif