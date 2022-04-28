/*
 * proxyRecer.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_PROXYRECER_H_
#define WORKSPACE_TRADER_INFRA_PROXYRECER_H_

#include "trader/infra/define.h"
#include <vector>
struct ZmqBase;

struct ProxyRecer
{
    void init();
    void * query_information_routine(void *context);
    void * order_routine(void *context);
    bool checkSessionAndTitle(std::vector<std::string>& sessionAndTitle);
    bool isTopicInSubTopics(std::string);
    std::vector<std::string> topicList;
    std::vector<std::string> queryTopicList;
    std::vector<std::string> orderTopicList;
};



#endif /* WORKSPACE_TRADER_INFRA_PROXYRECER_H_ */
