/*
 * direct_sender.h
 *
 *  Created on: 2020年08月29日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_DIRECTSENDER_H_
#define WORKSPACE_TRADER_INFRA_DIRECTSENDER_H_
#include <mutex>
#include "common/self/utils.h"

struct DirectSender {
 public:
  DirectSender();
  ~DirectSender();
  bool SendMsg(utils::ItpMsg &msg);

 private:
  std::string pub_ipaddport_;
  void *publisher_{nullptr};
};
#endif /* WORKSPACE_TRADER_INFRA_DIRECTSENDER_H_ */
