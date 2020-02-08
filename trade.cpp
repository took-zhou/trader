#include "main.h"
#include "traderSpi.h"
#include <sys/types.h>  //contains definitions of a number of data types used in system calls
#include <sys/socket.h> // includes a number of definitions of structures needed for sockets
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses.
#include <pthread.h>
#include <thread>
#include <semaphore.h>
//#include "utils.h"
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

std::mutex mtx;
int main() {

    /**********键盘监控zxc联按退出整个程序************/
    std::thread keyboardMonitor(monitorKeyBoard); // @suppress("Type cannot be resolved")
    Trader& trader = Trader::getInstance();
//    if( atoi(getConfig("trade","dbgSwitch").c_str()) == 0)
    LOG_INIT("log", "tradelog", 6);
    go [&] {
        SELF(trader, LogInPart).startLoginOutControl();
        SELF(trader, HandleSel).msgHandleSel();
    };
    std::thread t([]{ co_sched.Start(0,2); });
    //t.detach();
    t.join();



    return 0;
}
