#include "main.h"
#include "traderSpi.h"
#include <sys/types.h>  //contains definitions of a number of data types used in system calls
#include <sys/socket.h> // includes a number of definitions of structures needed for sockets
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses.
#include <pthread.h>
#include <thread>
#include <semaphore.h>
#include "utils.h"
#include "trade_components/query.h"
#include<string.h>
#include "log.h"
#include "keyboard.h"
#include "trader.h"
#include "dci/Role.h"
#include "coroutine.h"
#include "trade_components/orderstates.h"
#include <iostream>
#include <mutex>
#include <atomic>
#include <unistd.h>
#include "define.h"

extern U8 debugSwitch;

std::mutex mtx;
int main() {
    if(debugSwitch == 1)
    {
        std::thread keyboardMonitor(monitorKeyBoard);
    }
    std::string tradeLogPath = getConfig("trade","LogPath");
    utils::creatFolder(tradeLogPath);
    LOG_INIT(tradeLogPath.c_str(), "tradelog", 6);
    INFO_LOG("TRADE LOG PATH is %s",tradeLogPath.c_str());
    DEBUG_LOG("%s", "version debug 20200620:1939");
    Trader& trader = Trader::getInstance();

    go [&] {
        SELF(trader,TraderInteractor).start();
    };
    go [&] {
        SELF(trader, SocketClient).init();
    };
    go [&] {
        SELF(trader, LogInPart).startLoginOutControl();
    };
    go [&] {
        SELF(trader, HandleSel).msgHandleSel();
    };
    std::thread t([]{ co_sched.Start(0,128);});
    //t.detach();
    t.join();

    return 0;
}


