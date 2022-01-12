#include "trader/domain/components/orderstates.h"
#include "common/extern/log/log.h"

bool OrderStates::insertState(const std::string& key, const char& state)
{
    if(orderKey2StateMap.find(key) == orderKey2StateMap.end())
    {
        WARNING_LOG("the key [%s] is not in orderKey2StateMap", key.c_str());
        if(not insertOrderKey(key))
        {
            ERROR_LOG("insert new key error!");
            return false;
        }
    }
    orderKey2StateMap.at(key)->push_back(state);
    INFO_LOG("new order state [%c] add success",state);
    showOrderStates(key);
    return true;
}

bool OrderStates::delLastState(const std::string& key)
{
    if(orderKey2StateMap.find(key) != orderKey2StateMap.end())
    {
        ERROR_LOG("the key [%s] is not in orderKey2StateMap", key.c_str());
        return false;
    }
    orderKey2StateMap.at(key)->pop_back();
    INFO_LOG("order state del success");
    return true;
}

bool OrderStates::insertOrderKey(std::string key)
{
    if(orderKey2StateMap.find(key) != orderKey2StateMap.end())
    {
        WARNING_LOG("the order key has exist");
        return false;
    }
    charVector* newKey = new charVector();
    orderKey2StateMap.insert({key, newKey});
    INFO_LOG("new order key [%s] add success",key.c_str());
    return true;
}

bool OrderStates::delOrderKey(std::string key)
{
    if(orderKey2StateMap.find(key) == orderKey2StateMap.end())
    {
        WARNING_LOG("the order key not exist");
        return false;
    }
    delete orderKey2StateMap.at(key);
    orderKey2StateMap.at(key) = nullptr;
    orderKey2StateMap.erase(key);
    INFO_LOG("order key del success");
    return true;
}

void OrderStates::showAllOrderStates()
{
    if(size() == 0)
    {
        WARNING_LOG("no order response");
        return;
    }
    INFO_LOG("showAllOrderStates:");
    for(const auto& orderRecord : orderKey2StateMap)
    {
        PURE_LOG("************************%s:*************************",orderRecord.first.c_str());
        std::string states{""};
        if(orderRecord.second->size() == 0)
        {
            continue;
        }
        for(const auto& state : *(orderRecord.second))
        {
            states.push_back(state);
            states += string("->");
        }
        states.pop_back();
        states.pop_back();
        PURE_LOG("%s", states.c_str());
        PURE_LOG("************************************************");
    }
    INFO_LOG("showAllOrderStates end");
    PURE_LOG("\n\n");
}

void OrderStates::showOrderStates(const std::string& key)
{
    if(size() == 0)
    {
        WARNING_LOG("no order response");
        return;
    }
    INFO_LOG("showOrderStates:");
    auto orderRecord = orderKey2StateMap.find(key);
    if(orderRecord != orderKey2StateMap.end())
    {
        PURE_LOG("************************%s:*************************",orderRecord->first.c_str());
        std::string states{""};
        for(const auto& state : *(orderRecord->second))
        {
            states.push_back(state);
            states += string("->");
        }
        states.pop_back();
        states.pop_back();
        PURE_LOG("%s", states.c_str());
        PURE_LOG("************************************************");
    }
    // INFO_LOG("showAllOrderStates end");
    // PURE_LOG("\n\n");
}

size_t OrderStates::size()
{
    return (size_t)orderKey2StateMap.size();
}

namespace orderkey
{
    using namespace std;
    std::string getOrderKey(CThostFtdcOrderField *pOrder)
    {
        std::string frontId = to_string(pOrder->FrontID);
        std::string sessionId = to_string(pOrder->SessionID);
        std::string orderRef = string(pOrder->OrderRef);
        std::string instrumentId  = string(pOrder->InstrumentID);
        std::string connect_sym = string("_");
        std::string orderKey = instrumentId+connect_sym+ \
                frontId + connect_sym + sessionId + \
                connect_sym+orderRef;
        return orderKey;
    }

}


