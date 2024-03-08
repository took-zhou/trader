/*
 * recer_sender.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_RECERSENDER_H_
#define WORKSPACE_TRADER_INFRA_RECERSENDER_H_

#include "trader/infra/recer/direct_recer.h"
#include "trader/infra/recer/inner_recer.h"
#include "trader/infra/recer/proxy_recer.h"
#include "trader/infra/sender/direct_sender.h"
#include "trader/infra/sender/email_sender.h"
#include "trader/infra/sender/inner_sender.h"
#include "trader/infra/sender/itp_sender.h"
#include "trader/infra/sender/proxy_sender.h"

#include <thread>
#include "common/self/dci/role.h"

struct Recer : InnerRecer, ProxyRecer, DirectRecer {
  IMPL_ROLE(InnerRecer);
  IMPL_ROLE(ProxyRecer);
  IMPL_ROLE(DirectRecer);
};

struct Sender : ItpSender, EmailSender, ProxySender, InnerSender, DirectSender {
  IMPL_ROLE(ItpSender);
  IMPL_ROLE(EmailSender);
  IMPL_ROLE(ProxySender);
  IMPL_ROLE(InnerSender);
  IMPL_ROLE(DirectSender);
};

struct RecerSender : Recer, Sender {
  RecerSender(){};
  RecerSender(const RecerSender &) = delete;
  RecerSender &operator=(const RecerSender &) = delete;
  static RecerSender &GetInstance() {
    static RecerSender instance;
    return instance;
  }

  void Run() {
    while (1) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }

  IMPL_ROLE(Recer);
  IMPL_ROLE(Sender);
};

#endif /* WORKSPACE_TRADER_INFRA_RECERSENDER_H_ */
