/*
 * define.cpp
 *
 *  Created on: 2020��9��1��
 *      Author: Administrator
 */

#include "trader/infra/define.h"
#include <map>
#include "common/extern/libgo/libgo/libgo.h"

std::map<std::string, EventType> TitleToEvent;

static MsgStruct NilMsgStruct;

co_chan<MsgStruct> ctpMsgChan;
