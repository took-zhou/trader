#ifndef LOGIN_PART_H
#define LOGIN_PART_H

#include <semaphore.h>
#include "dci/Role.h"
#include "ctpClient.h"
#include "trade_components/trader_commom.h"
#include <thread>
#include "trade_components/settlementConfirm.h"
enum struct CtpLogInState
{
    Prepare_State     = 0,
    InitFailed_State  = 1,
    Connected_State   = 2,
    Invalid_State     = 3
};

struct LogInPart
{
    bool logIn();
    bool logOut();
    static void loginAndLogoutControl(LogInPart* _this);
//    void goLoginAndLogoutControl();
    void startLoginOutControl();
    CtpLogInState getCtpLogInState();
public:

    std::thread logInOutThread;

    bool isLogIN {false};
    bool isLogInThreadRunning{false};
    bool isForceExitThreadRuning{ false };
    USE_ROLE(CtpClient);
    USE_ROLE(Trader_Info);
    USE_ROLE(SettlementConfirm);
};
#endif // LOGIN_PART_H
