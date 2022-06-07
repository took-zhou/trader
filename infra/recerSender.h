/*
 * recerSender.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_RECERSENDER_H_
#define WORKSPACE_TRADER_INFRA_RECERSENDER_H_

#include "trader/infra/sender/ctpSender.h"
#include "trader/infra/sender/emailSender.h"
#include "trader/infra/sender/interactSender.h"
#include "trader/infra/sender/proxySender.h"

#include "trader/infra/recer/ctpRecer.h"
#include "trader/infra/recer/interactRecer.h"
#include "trader/infra/recer/proxyRecer.h"
#include "trader/infra/zmqBase.h"

#include "common/extern/log/log.h"

struct Recer : CtpRecer,
               InteractRecer,
               ProxyRecer

{
  bool init() {
    ROLE(CtpRecer).init();
    ROLE(InteractRecer).init();
    ROLE(ProxyRecer).init();
    return true;
  };
  IMPL_ROLE(CtpRecer);
  IMPL_ROLE(InteractRecer);
  IMPL_ROLE(ProxyRecer);
};

struct Sender : CtpSender, InteractSender, EmailSender, ProxySender {
  bool init() {
    ROLE(CtpSender).init();
    ROLE(InteractSender).init();
    ROLE(EmailSender).init();
    ROLE(ProxySender).init();
    return true;
  };
  IMPL_ROLE(CtpSender);
  IMPL_ROLE(InteractSender);
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
  bool init() {
    ROLE(Recer).init();
    INFO_LOG("Recer init ok");
    ROLE(Sender).init();
    return true;
  }

  bool run() {
    // Turn on the accept proxy, called at the end of the main process
    auto &zmqBase = ZmqBase::getInstance();
    zmq_proxy(zmqBase.receiver, zmqBase.workers, NULL);
  }

  IMPL_ROLE(Recer);
  IMPL_ROLE(Sender);
};

#endif /* WORKSPACE_TRADER_INFRA_RECERSENDER_H_ */
