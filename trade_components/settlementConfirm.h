#ifndef __SETTLEMENTCONFIRM_H__
#define __SETTLEMENTCONFIRM_H__
#include "main.h"
#include <unistd.h>
#include "dci/Role.h"
#include "ctpClient.h"

struct SettlementConfirm
{
    bool confirm();
    bool confirmToday(CSimpleHandler& pTraderApi);
    bool hasConfirmedToday();

private:
    size_t nRequestID{1};
public:
    USE_ROLE(CtpClient);

};

#endif
