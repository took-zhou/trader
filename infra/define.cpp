/*
 * define.cpp
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */
#include <map>

#include "trader/infra/define.h"
#include "common/extern/libgo/libgo/libgo.h"

std::map<std::string, EventType> TitleToEvent;

static MsgStruct NilMsgStruct;

co_chan<MsgStruct> ctpMsgChan;
co_chan<MsgStruct> orderMsgChan;
co_chan<MsgStruct> queryMsgChan;
