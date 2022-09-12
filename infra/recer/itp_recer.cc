#include "trader/infra/recer/itp_recer.h"
#include "common/self/file_util.h"
#include "trader/infra/inner_zmq.h"

ItpRecer::ItpRecer() {}

bool ItpRecer::ReceMsg(utils::ItpMsg &msg) {
  auto &innerBaseZmq = InnerZmq::getInstance();

  return innerBaseZmq.PullTask(msg);
}
