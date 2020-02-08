#ifndef CTPCLIENT_H
#define CTPCLIENT_H
//#include "traderSpi.h"
#include "main.h"

struct CtpClient
{
//    CtpClient();

    void init();

    void release();

public:
    CTraderApi* pUserApi {nullptr};
    CSimpleHandler sh{0};

};


#endif
