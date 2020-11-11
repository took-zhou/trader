/*
 * InsertResult.h
 *
 *  Created on: 2020年10月23日
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_DOMAIN_COMPONENTS_INSERTRESULT_H_
#define WORKSPACE_TRADER_DOMAIN_COMPONENTS_INSERTRESULT_H_

#include <map>
#include <string>

enum struct InsertRspResult
{
    Success        = 0,
    Failed         = 1,
    Order_Canceled = 2,
    UnKnow         = 3

};

using RspResultMap = std::map<std::string, InsertRspResult>;
using RspResultPair = std::pair<std::string, InsertRspResult>;

struct InsertResult
{
    InsertResult(){};
    InsertResult(const InsertResult&) = delete;
    InsertResult& operator=(const InsertResult&) = delete;
    static InsertResult& getInstance()
    {
        static InsertResult instance;
        return instance;
    }

    bool addResultMonitor(std::string orderRef);
    void delResultMonitor(std::string orderRef);
    bool setResultState(std::string orderRef, InsertRspResult result);
    InsertRspResult getRspMonitorResult(std::string orderRef);

private:
    RspResultMap rspResultMap;
};



#endif /* WORKSPACE_TRADER_DOMAIN_COMPONENTS_INSERTRESULT_H_ */
