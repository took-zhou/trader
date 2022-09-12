/*
 * recer_sender.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_RECERSENDER_H_
#define WORKSPACE_TRADER_INFRA_RECERSENDER_H_

#include "trader/infra/sender/email_sender.h"
#include "trader/infra/sender/itp_sender.h"
#include "trader/infra/sender/proxy_sender.h"

#include "trader/infra/base_zmq.h"
#include "trader/infra/recer/itp_recer.h"
#include "trader/infra/recer/proxy_recer.h"

#include "common/extern/log/log.h"

struct Recer : ItpRecer, ProxyRecer {
  IMPL_ROLE(ItpRecer);
  IMPL_ROLE(ProxyRecer);
};

struct Sender : ItpSender, EmailSender, ProxySender {
  IMPL_ROLE(ItpSender);
  IMPL_ROLE(EmailSender);
  IMPL_ROLE(ProxySender);
};

struct RecerSender : Recer, Sender {
  RecerSender(){};
  RecerSender(const RecerSender &) = delete;
  RecerSender &operator=(const RecerSender &) = delete;
  static RecerSender &getInstance() {
    static RecerSender instance;
    return instance;
  }

  bool Run() {
    // Turn on the accept proxy, called at the end of the main process
    auto &zmqBase = BaseZmq::getInstance();
    zmq_proxy(zmqBase.receiver, zmqBase.workers, NULL);
  }

  IMPL_ROLE(Recer);
  IMPL_ROLE(Sender);
};

#endif /* WORKSPACE_TRADER_INFRA_RECERSENDER_H_ */
