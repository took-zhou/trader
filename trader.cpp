#include "trader.h"
#include "log.h"
#include <stdio.h>
#include <unistd.h>
#include "coroutine.h"
#include <pthread.h>

void Trader::init()
{
   go [&] {ROLE(SocketClient).init();};
   go [&] {ROLE(LogInPart).startLoginOutControl();};
   std::thread threadInit([]{ co_sched.Start(0,4); });
       //t.detach();
   threadInit.join();
}
