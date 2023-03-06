/*
 * direct_recer.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_DIRECTRECER_H_
#define WORKSPACE_TRADER_INFRA_DIRECTRECER_H_

#include <vector>
#include "common/self/utils.h"

struct DirectRecer {
 public:
  DirectRecer();
  bool ReceMsg(utils::ItpMsg &msg);
  bool IsTopicInSubTopics(std::string);
  std::vector<std::string> topic_list;

 private:
  void SubscribeTopic();
  void UnSubscribeTopic();
  void *receiver_{nullptr};
};

#endif /* WORKSPACE_TRADER_INFRA_PROXYRECER_H_ */
