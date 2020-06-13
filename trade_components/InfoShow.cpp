#include "InfoShow.h"
#include "ThostFtdcUserApiStruct.h"
#include <iostream>
#include "semaphorePart.h"
#include "log.h"
#include "getconfig.h"
#include <mutex>
#include "keyboard.h"
extern GlobalSem globalSem;
//atomic<InfoShowFlag> infoShowFlag;
InfoShowFlag infoShowFlag;
CThostFtdcInstrumentField InstrumentInfo;
extern std::mutex mtx;
namespace
{
    vector<string> stringSplit(const string &str, const string &pattern)
    {
        vector<string> res;
        if(str == "")
            return res;
        //在字符串末尾也加入分隔符，方便截取最后一�??
        string strs = str + pattern;
        size_t pos = strs.find(pattern);
        while(pos != strs.npos)
        {
            string temp = strs.substr(0, pos);
            res.push_back(temp);
            //去掉已分割的字符�??,在剩下的字符串中进行分割
            strs = strs.substr(pos+1, strs.size());
            pos = strs.find(pattern);
        }
        return res;
}
}
void InfoShow::infoShowTest(string command)
{
    if(!ROLE(LogInPart).isLogIN){
        WARNING_LOG("%s","ctp not login!");
        return;
    }
//        string command = getConfigFromFile("../../project/projroot/queryDbg.ini","query","Command");
    INFO_LOG("test command is:%s",command.c_str());
    auto splitedStr = stringSplit(command, string(" "));
    for(auto iter : splitedStr)
    {
        WARNING_LOG("%s",iter.c_str());
    }
    if(!cmdSelect(splitedStr))
    {
        ERROR_LOG("%s","query info show error!");
        return;
    }
    INFO_LOG("%s test ok!",command.c_str());

}

bool InfoShow::cmdSelect(const vector<string> splitedStr)
{
    if(!ROLE(LogInPart).isLogIN)
    {
        ERROR_LOG("can't query, ctp client does not logged in");
        return false;
    }
    string cmd = splitedStr.at(0);
    if(cmd == "query")
    {
        string instrumentId = splitedStr.at(1);
        string exchangeId = splitedStr.at(2);
        singleInstrumentQuery(instrumentId, exchangeId);
        return true;
    }
    return false;
}

void InfoShow::singleInstrumentQuery(string& instrumentId, string& exchangeId)
{
    CThostFtdcQryInstrumentField reqBody = { 0 };
    std::strcpy(reqBody.InstrumentID, instrumentId.c_str());
    std::strcpy(reqBody.ExchangeID, exchangeId.c_str());
    infoShowFlag.SingleInstrumentQuery = true;
    ROLE(CtpClient).sh.ReqQryInstrument_hai(reqBody);
    sem_wait(&globalSem.sem_query);
    infoShowFlag.SingleInstrumentQuery = false;
    ROLE(PintCheck).printInstrumentInfo(InstrumentInfo);
}
