#include "ctpClient.h"
#include "utils.h"

void CtpClient::init()
{
    pUserApi = new CTraderApi;     //新建tradeAPI
    std::string conPath = getConfig("trade", "ConPath");
    utils::creatFolder(conPath);
    pUserApi->CreateFtdcTraderApi(conPath.c_str()); //设置.con的保存位置
//        CSimpleHandler sh(pUserApi);     //初始化回调类
    sh = CSimpleHandler(pUserApi);
    pUserApi->RegisterSpi(&sh);      //注册回调类
    pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);   //订阅
    pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);    //订阅

    std::string g_chFrontaddr = getConfig("trade", "FrontAddr");
    pUserApi->RegisterFront(const_cast<char *>(g_chFrontaddr.c_str()));  //注册前置地址
}

void CtpClient::release()
{
    pUserApi->Release();
    pUserApi = nullptr;
}
