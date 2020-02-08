#include "settlementConfirm.h"
#include "json.h"
#include <fstream>
#include "log.h"
#include<time.h>
//#include<string>
#include <semaphore.h>
#include<vector>
#include<iostream>
#include <iomanip>
#include "semaphorePart.h"
extern GlobalSem globalSem;

using json = nlohmann::json;
using namespace std;
inline bool is_file_exists (const std::string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}

namespace
{
    std::unordered_map<string, string> MonthMap = {
            {"Jan","01"},
            {"January ","01"},
            {"Feb","02"},
            {"February","02"},
            {"Mar","03"},
            {"March","03"},
            {"Apr","04"},
            {"April","04"},
            {"May","05"},
            {"Jun","06"},
            {"June","06"},
            {"Jul","07"},
            {"July","07"},
            {"Aug","08"},
            {"August","08"},
            {"Sept","09"},
            {"Septemper","09"},
            {"Oct","10"},
            {"October","10"},
            {"Nov","11"},
            {"November","11"},
            {"Dec","12"},
            {"December","12"},
    };
    bool removeSpecialStrInTheEnd(string &str, const char &pattern )
    {
        int string_len = str.length();
        if(str.at(string_len-1) == pattern)
        {
            str = str.substr(0,string_len-1);
        }
        return true;
    }
    vector<string> split(const string &str, const string &pattern)
    {
        vector<string> res;
        if(str == "")
            return res;
        string strs = str + pattern;
        size_t pos = strs.find(pattern);
        while(pos != strs.npos)
        {
            string temp = strs.substr(0, pos);
            res.push_back(temp);
            strs = strs.substr(pos+1, strs.size());
            pos = strs.find(pattern);
        }
        return res;
    }
    string getDate()
    {
        time_t now_time = time(NULL);
            //local time
        tm* local_time = localtime(&now_time);
        vector<string> split_strings = split(asctime(local_time),string(" "));
        int size = split_strings.size();
        string year = split_strings[size-1];
        if(!removeSpecialStrInTheEnd(year,'\n'))
        {
            printf("orror in removeSpecialStrInTheEnd\n"); // @suppress("Invalid arguments")
        }
        string month = MonthMap[split_strings[1]];
        string day = split_strings[2];

        return year+month+day;
    }
}

bool SettlementConfirm::confirm()
{
    CSimpleHandler& pTraderApi = ROLE(CtpClient).sh;
    if(!hasConfirmedToday())
    {
        INFO_LOG("begin to confirm settlement today");
        if(confirmToday(pTraderApi))
        {
            INFO_LOG("confirmToday ok!");
            return true;
        }
        ERROR_LOG("confirm today error");
    }
    return true;
}

bool SettlementConfirm::hasConfirmedToday()
{
    json record;
    string confirmRecord = "../../project/projroot/confirmRecord.json";
    if(is_file_exists(confirmRecord))
    {
        ifstream istrm(confirmRecord,ios::binary);
        if (!istrm.is_open())
        {
            ERROR_LOG("failed to open %s",confirmRecord.c_str()); // @suppress("Invalid arguments")
        }
        istrm >> record;
        istrm.close();
        JsonPrint(record);

        if(record["date"].get<string>() == getDate()
           && record["value"].get<int>() == 1)
        {
            INFO_LOG("settlement today has been confirmed before!");
            return true;
        }
        return false;
    }
    return false;
}

bool SettlementConfirm::confirmToday(CSimpleHandler& pTraderApi)
{
    json orderCfg;
    string cfgFileName = "../../project/projroot/commonorder.json";
    ifstream istrm(cfgFileName,ios::binary);
    if (!istrm.is_open())
    {
        printf("failed to open %s",cfgFileName.c_str());
    }
    istrm >> orderCfg;
    istrm.close();

    CThostFtdcSettlementInfoConfirmField requestMsg{0};
    string brokerId = orderCfg["BrokerID"].get<string>();
    strcpy(requestMsg.BrokerID, brokerId.c_str());

    string investorId = orderCfg["InvestorID"].get<string>();
    strcpy(requestMsg.InvestorID, investorId.c_str());

    json record;
    record["date"] = getDate();
    record["value"] = 0;

    string confirmRecord = "../../project/projroot/confirmRecord.json";
    ofstream ostrm(confirmRecord,ios::trunc);
    ostrm << setw(4) << record << endl;
    ostrm.close();
    pTraderApi.ReqSettlementInfoConfirm(&requestMsg, nRequestID++);
    sem_wait(&globalSem.sem);

    record["value"] = 1;
    ofstream ostrm_set_1(confirmRecord,ios::trunc);
    ostrm_set_1 << setw(4) << record << endl;
    ostrm_set_1.close();

    return true;
}
