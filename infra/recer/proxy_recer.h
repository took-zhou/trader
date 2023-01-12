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
  bool ReceQueryMsg(utils::ItpMsg &msg);
  bool ReceOrderMsg(utils::ItpMsg &msg);
  bool IsTopicInQuerySubTopics(std::string);
  bool IsTopicInOrderSubTopics(std::string);
  std::vector<std::string> query_topic_list;
  std::vector<std::string> order_topic_list;

 private:
  bool InitQueryReceiver(void);
  bool InitOrderReceiver(void);
  void SubscribeQueryTopic();
  void UnSubscribeQueryTopic();
  void SubscribeOrderTopic();
  void UnSubscribeOrderTopic();
  void *query_receiver_{nullptr};
  void *order_receiver_{nullptr};
};

#endif /* WORKSPACE_TRADER_INFRA_PROXYRECER_H_ */
