#ifndef __SETTLEMENTCONFIRM_H__
#define __SETTLEMENTCONFIRM_H__
#include <unistd.h>
#include "common/self/dci/Role.h"
#include <string>
struct CtpTraderApi;

struct SettlementConfirm
{
    bool confirm(const std::string tradingDay);
    bool confirmToday(const std::string tradingDay);
    bool hasConfirmedToday(const std::string tradingDay);

private:
    size_t nRequestID{1};
public:
    USE_ROLE(CtpTraderApi);

};

#endif
