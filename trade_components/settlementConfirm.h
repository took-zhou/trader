#ifndef __SETTLEMENTCONFIRM_H__
#define __SETTLEMENTCONFIRM_H__
#include "main.h"
#include <unistd.h>
#include "dci/Role.h"
#include "ctpClient.h"

struct SettlementConfirm
{
    bool confirm(const std::string tradingDay);
    bool confirmToday(const std::string tradingDay, CSimpleHandler& pTraderApi);
    bool hasConfirmedToday(const std::string tradingDay);

private:
    size_t nRequestID{1};
public:
    USE_ROLE(CtpClient);

};

#endif
