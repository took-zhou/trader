#include <termio.h>
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include "log.h"
#include <string>
#include "trader.h"
#include "orderstates.h"

namespace
{
    vector<string> stringSplit(const string &str, const string &pattern)
    {
        vector<string> res;
        if(str == "")
            return res;
        //鍦ㄥ瓧绗︿覆鏈熬涔熷姞鍏ュ垎闅旂锛屾柟渚挎埅鍙栨渶鍚庝竴娈�
        string strs = str + pattern;
        size_t pos = strs.find(pattern);
        while(pos != strs.npos)
        {
            string temp = strs.substr(0, pos);
            res.push_back(temp);
            //鍘绘帀宸插垎鍓茬殑瀛楃涓�,鍦ㄥ墿涓嬬殑瀛楃涓蹭腑杩涜鍒嗗壊
            strs = strs.substr(pos+1, strs.size());
            pos = strs.find(pattern);
        }
        return res;
}
}

int scanKeyboard()
{
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0, &stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);

    in = getchar();

    tcsetattr(0, TCSANOW, &stored_settings);
    return in;
}
//这个方法就可以，返回值是该键的ASCII码值，不需要回车的，


namespace
{
    void showState(const Trader& trader)
    {
        INFO_LOG("router login state:%s", trader.ROLE(SocketClient).isRouterConnected ? "true":"false");
        INFO_LOG("ctp login state:%s\n", trader.ROLE(LogInPart).isLogIN ? "true":"false");
    }
}
void monitorKeyBoard()
{
    std::string command;
    while (true)
    {
        INFO_LOG("command read is ready......");
        command = "";
        std::cin.clear();  // 更改cin的状态标示符
        std::cin.sync();  // 清除缓存区的数据流
        getline(std::cin, command); //读取一行字符
//        std::cout << command << std::endl;
        INFO_LOG("command:%s",command.c_str());
        auto splitedStr = stringSplit(command, string(" "));
        if(splitedStr.size() == 0)
        {
            continue;
        }
        if(splitedStr.at(0) == string("quit"))
        {
            exit(0);
            return;
        }
        if(splitedStr.at(0) == string("showOrders"))
        {
            Trader& trader = Trader::getInstance();
            auto& orderState = OrderStates::getInstance();
            orderState.showAllOrderStates();
            continue;
        }
        if(splitedStr.at(0) == string("query"))
        {
            Trader& trader = Trader::getInstance();
            trader.ROLE(InfoShow).infoShowTest(command);
            continue;
        }
        if(splitedStr.at(0) == string("showStates"))
       {
           Trader& trader = Trader::getInstance();
           showState(trader);
           continue;
       }
        if(splitedStr.at(0) == string("help"))
        {
            INFO_LOG("%s\n"," quit\n showorders\n query+instrumentID+exchangeId\n showStates");
            continue;
        }

        ERROR_LOG("Invalid command:%s",splitedStr.at(0).c_str());
    }
}
