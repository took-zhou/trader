#ifndef ORDERSTATES_H
#define ORDERSTATES_H
#include <unordered_map>
#include <string>
#include <vector>
#include <stdint.h>
#include "ThostFtdcUserApiStruct.h"
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

    size_t size();

    bool insertOrderKey(std::string key);
public:
    std::unordered_map<std::string, charVector*> orderKey2StateMap;
};

namespace orderkey
{
    using namespace std;
    string getOrderKey(CThostFtdcOrderField *pOrder);
}
void showOrderStates();
#endif // ORDERSTATES_H
