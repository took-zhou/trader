#ifndef LOGIN_PART_H
#define LOGIN_PART_H

#include <semaphore.h>
#include "dci/Role.h"
#include "ctpClient.h"
#include "trade_components/trader_commom.h"
#include <thread>
#include "trade_components/settlementConfirm.h"
struct LogInPart
{
    bool logIn();
    bool logOut();
    static void loginAndLogoutControl(LogInPart* _this);
    void goLoginAndLogoutControl();
    void startLoginOutControl();

public:

    std::thread logInOutThread;

    bool isLogIN {false};
    USE_ROLE(CtpClient);
    USE_ROLE(Trader_Info);
    USE_ROLE(SettlementConfirm);
};
#endif // LOGIN_PART_H
