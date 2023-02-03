/*
 * email_sender.h
 *
 *  Created on: 2022.04.28
 *      Author: Administrator
 */
#ifndef WORKSPACE_TRADER_INFRA_EMAILSENDER_H_
#define WORKSPACE_TRADER_INFRA_EMAILSENDER_H_

#include "common/extern/csmtp/csmtp.h"

struct EmailSender {
 public:
  EmailSender();
  void Send(const char *head, const char *msg);
};

#endif /* WORKSPACE_TRADER_INFRA_EMAILSENDER_H_ */
