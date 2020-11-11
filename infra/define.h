/*
 * define.h
 *
 *  Created on: 2020��8��31��
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_DEFINE_H_
#define WORKSPACE_TRADER_INFRA_DEFINE_H_

#include <string>
#include "common/extern/ctp/inc/ThostFtdcTraderApi.h"
enum struct EventType
{
    Ctp_Event           = 0,
    Market_Event        = 1,
    Trader_Event        = 2,
    Strategy_Event      = 3,
    Interactor_Event    = 4,
    INVALID             = 5
};

struct SpecialMsg
{
    CThostFtdcInstrumentField instrumentField;
};

struct MsgStruct
{
    std::string sessionName{""};
    std::string msgName{""};
    std::string pbMsg{""};
    void* ctpMsg{nullptr};
    void* ctpMsgInfo{nullptr};
    bool bIsLast{false};

    SpecialMsg specialMsg;
    bool isValid()
    {
        return sessionName != std::string("") and msgName != std::string("");
    }
};

#endif /* WORKSPACE_STRATEGY_INFRA_DEFINE_H_ */
