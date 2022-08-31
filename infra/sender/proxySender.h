/*
 * proxySender.h
 *
 *  Created on: 2020年08月29日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_PROXYSENDER_H_
#define WORKSPACE_TRADER_INFRA_PROXYSENDER_H_

struct ProxySender {
  ProxySender(){};
  bool send(const char *head, const char *msg);
};

#endif /* WORKSPACE_TRADER_INFRA_PROXYSENDER_H_ */
