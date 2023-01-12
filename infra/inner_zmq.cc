/*
 * innerZmq.cpp
 *
 *  Created on: 2021年2月17日
 *      Author: Administrator
 */

#include "trader/infra/inner_zmq.h"
#include "common/extern/log/log.h"

InnerZmq::InnerZmq() { context = zmq_ctx_new(); }
