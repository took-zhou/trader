/*
 * proxyRecer.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_PROXYRECER_H_
#define WORKSPACE_TRADER_INFRA_PROXYRECER_H_

#include <vector>
#include "common/self/utils.h"
#include "trader/infra/zmqBase.h"

struct ProxyRecer {
 public:
  ProxyRecer();
  bool receQueryMsg(utils::ItpMsg &msg);
  bool receOrderMsg(utils::ItpMsg &msg);
  bool isTopicInSubTopics(std::string);

  std::vector<std::string> topicList;
  std::vector<std::string> queryTopicList;
  std::vector<std::string> orderTopicList;

 private:
  bool initQueryReceiver(void);
  bool initOrderReceiver(void);
  char *queryRecv();
  char *orderRecv();
  void *query_receiver{nullptr};
  void *order_receiver{nullptr};
};

#endif /* WORKSPACE_TRADER_INFRA_PROXYRECER_H_ */
