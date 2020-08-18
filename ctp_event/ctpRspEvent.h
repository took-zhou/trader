/*
 * ctpRspEvent.h
 *
 *  Created on: 2020Äê8ÔÂ8ÈÕ
 *      Author: Administrator
 */
#include "ctp_event/reqRspQryTradingAccountHandle.h"
#include "dci/Role.h"
#ifndef WORKSPACE_TRADE_CTP_EVENT_CTPRSPEVENT_H_
#define WORKSPACE_TRADE_CTP_EVENT_CTPRSPEVENT_H_
#include <string>
struct MsReqCtpRspEvent:ReqRspQryTradingAccountHandle
{
    MsReqCtpRspEvent();
    MsReqCtpRspEvent(const MsReqCtpRspEvent&) = delete;
    MsReqCtpRspEvent& operator=(const MsReqCtpRspEvent&) = delete;
    static MsReqCtpRspEvent& getInstance()
    {
        static MsReqCtpRspEvent instance;
        return instance;
    }
    IMPL_ROLE(ReqRspQryTradingAccountHandle);
};


#endif /* WORKSPACE_TRADE_CTP_EVENT_CTPRSPEVENT_H_ */
