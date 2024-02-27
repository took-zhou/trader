/*
 * proxy_recer.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_PROXYRECER_H_
#define WORKSPACE_TRADER_INFRA_PROXYRECER_H_

#include <vector>
#include "common/self/utils.h"

struct ProxyRecer {
 public:
  ProxyRecer();
  bool ReceMsg(utils::ItpMsg &msg);
  bool IsTopicInSubTopics(const std::string &title);

 private:
  void SubscribeTopic();
  void UnSubscribeTopic();
  std::vector<std::string> topic_list_;
  void *receiver_{nullptr};
};

#endif /* WORKSPACE_TRADER_INFRA_PROXYRECER_H_ */
