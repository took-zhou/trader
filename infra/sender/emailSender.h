/*
 * emailSender.h
 *
 *  Created on: 2022.04.28
 *      Author: Administrator
 */
#ifndef WORKSPACE_TRADER_INFRA_EMAILSENDER_H_
#define WORKSPACE_TRADER_INFRA_EMAILSENDER_H_
#include <string>

#include "common/extern/csmtp/CSmtp.h"

struct EmailSender {
  bool init();
  void send(const char *head, const char *msg);

  std::string mail_title;
  std::string mail_body;
};

#endif /* WORKSPACE_TRADER_INFRA_EMAILSENDER_H_ */
