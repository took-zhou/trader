/*
 * proxySender.h
 *
 *  Created on: 2020年08月29日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_PROXYSENDER_H_
#define WORKSPACE_TRADER_INFRA_PROXYSENDER_H_
#include "common/self/utils.h"

struct ProxySender {
  ProxySender(){};
  bool send(utils::ItpMsg &msg);
};

#endif /* WORKSPACE_TRADER_INFRA_PROXYSENDER_H_ */
