
#include<time.h>
#include<vector>
#include<iostream>
#include<sstream>
#include<unordered_map>
#include "OrderRef.h"
vector<string> split(const string &str, const string &pattern);
bool removeSpecialStrInTheEnd(string &str, const char &pattern );
bool addCntToOrderRef(string &orderRef, const unsigned int &cnt);

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

string genOrderRef()
{
    static unsigned int resCnt(1);
    string orderRef;
    if (resCnt > 9999)
    {
        resCnt = 1;
    }
    //system time
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
    //	vector<string> hour_minute_second =  split(split_strings[size-2],string(":"));
    //	string hour_minute = hour_minute_second[0] + hour_minute_second[1];
    //	orderRef = year+hour_minute; //year hour and minutes part are ok.
    orderRef = year+month+day;
    addCntToOrderRef(orderRef,resCnt);
    resCnt++;
    return  orderRef;
}

bool addCntToOrderRef(string &orderRef, const unsigned int &cnt)
{
    stringstream cntAsString;
    cntAsString << cnt;
    string cntString = cntAsString.str();
    while(cntString.length() < 4)
    {
        cntString = string("0") + cntString;
    }
    orderRef = orderRef + cntString;
    return true;
}

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
    //在字符串末尾也加入分隔符，方便截取最后一殄1�7
    string strs = str + pattern;
    size_t pos = strs.find(pattern);
    while(pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        res.push_back(temp);
        //去掉已分割的字符丄1�7,在剩下的字符串中进行分割
        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(pattern);
    }
    return res;
}
