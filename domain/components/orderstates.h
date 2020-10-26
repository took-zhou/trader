#ifndef ORDERSTATES_H
#define ORDERSTATES_H
#include <string>
#include <vector>
#include <stdint.h>
#include "common/extern/ctp/inc/ThostFtdcUserApiStruct.h"
#include <map>

using charVector = std::vector<char>;

struct OrderStates
{
    OrderStates(){}
    OrderStates(const OrderStates&)=delete;
    OrderStates& operator=(const OrderStates&)=delete;
    static OrderStates& getInstance()
    {
        static OrderStates instance;
        return instance;
    }

    bool insertState(const std::string& key, const char& state);
    bool delLastState(const std::string& key);
//    bool delState(const std::string& key, const char& state);

    bool delOrderKey(std::string key);

    void showAllOrderStates();
    void showOrderStates(const std::string& key);

    size_t size();

    bool insertOrderKey(std::string key);
public:
    std::map<std::string, charVector*> orderKey2StateMap;
};

namespace orderkey
{
    using namespace std;
    string getOrderKey(CThostFtdcOrderField *pOrder);
}
#endif // ORDERSTATES_H
