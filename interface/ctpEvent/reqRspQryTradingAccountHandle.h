/*
 * onRspQryTradingAccountHandle.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADE_CTP_EVENT_REQRSPQRYTRADINGACCOUNTHANDLE_H_
#define WORKSPACE_TRADE_CTP_EVENT_REQRSPQRYTRADINGACCOUNTHANDLE_H_

struct CThostFtdcTradingAccountField;

struct ReqRspQryTradingAccountHandle
{
    void reqQryTradingAccountHandle();
    void onRspQryTradingAccountHandle(CThostFtdcTradingAccountField& rsp);

};

#endif /* WORKSPACE_TRADE_CTP_EVENT_REQRSPQRYTRADINGACCOUNTHANDLE_H_ */
