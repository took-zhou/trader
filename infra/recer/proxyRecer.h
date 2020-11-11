/*
 * proxyRecer.h
 *
 *  Created on: 2020��8��29��
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
    MsgStruct receMsg();
    bool checkSessionAndTitle(std::vector<std::string>& sessionAndTitle);
    bool isTopicInSubTopics(std::string);
//    ZmqBase* zmq{nullptr};
    std::vector<std::string> topicList;
};



#endif /* WORKSPACE_TRADER_INFRA_PROXYRECER_H_ */
