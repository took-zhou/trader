/*
 * proxy_sender.h
 *
 *  Created on: 2020年08月29日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_PROXYSENDER_H_
#define WORKSPACE_TRADER_INFRA_PROXYSENDER_H_
#include <mutex>
#include "common/self/utils.h"

struct ProxySender {
 public:
  ProxySender();
  bool SendMsg(utils::ItpMsg &msg);

 private:
  void *publisher_{nullptr};
  std::mutex m_lock_;
};
#endif /* WORKSPACE_TRADER_INFRA_PROXYSENDER_H_ */
