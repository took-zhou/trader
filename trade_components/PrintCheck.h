#ifndef __PRINTCHECK__
#define __PRINTCHECK__

#include "dci/Role.h"
#include "ThostFtdcUserApiStruct.h"
#include "define.h"
#include "json.h"
#include <string>

using json = nlohmann::json;
using namespace std;
struct PintCheck
{
    void printMsgHead(const TradeMsgHead& msgHead);
    void printMsgBody(const json& msg){}
    void printOrderInfo(const CThostFtdcInputOrderField& order,  const string& orderName);
    void printRspResult(const json& resultMsg){}
    void printInstrumentInfo(const CThostFtdcInstrumentField& instrumentInfo);

};

#endif
