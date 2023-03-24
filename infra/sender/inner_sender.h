/*
 * inner_sender.h
 *
 *  Created on: 2020.11.13
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_INNERSENDER_H_
#define WORKSPACE_TRADER_INFRA_INNERSENDER_H_

#include <string>
#include "common/self/utils.h"

struct InnerSender {
 public:
  InnerSender();
  bool SendMsg(utils::ItpMsg &msg);

 private:
  void *pusher_{nullptr};
};

#endif /* WORKSPACE_TRADER_INFRA_INNERSENDER_H_ */
