
#ifndef __UTILS__
#define __UTILS__

#include "ThostFtdcTraderApi.h"
#include "main.h"

#define BUY '0'
#define SELL '1'
#define  ARBITRAGEDIRECTION_BUY1_SELL2 '0'    //��Լ1�򣬺�Լ2��
#define  ARBITRAGEDIRECTION_SELL1_BUY2 '1'    //��Լ1������Լ2��

// #define TRADE_STRATEGY_STRUCT_SIZE  600
typedef char SOCKET_PORT_TYPE[10];
typedef char SOCKET_IP_TYPE[20];
typedef enum
{
    OrderInsert = 1,
    OrderOperation
} ActionType;

typedef struct
{
    ActionType action_flag;
    char msg[124];

} TRADE_STRATEGY_FRONT_STRUCT;

typedef struct{
    TThostFtdcInstrumentIDType  InstrumentID;/*Contract code*/
    TThostFtdcPriceType         LimitPrice;/*Trading price*/
    TThostFtdcVolumeType        VolumeTotalOriginal;/*Trading volumn*/
}Contract;

typedef struct {
	TThostFtdcInstrumentIDType InstrumentID;
	TThostFtdcOrderPriceTypeType OrderPriceType;
	TThostFtdcDirectionType Direction;
	TThostFtdcCombOffsetFlagType CombOffsetFlag;
	TThostFtdcPriceType LimitPrice;
	TThostFtdcVolumeType VolumeTotalOriginal;
	TThostFtdcExchangeIDType ExchangeID;
}GDF_STRUCT;


void error(const char *msg);
void dostuff(int sock);
bool dealOrder_SA(int sock, CSimpleHandler& pTraderApi);
void dealOrder_GDF(int sock, CTraderApi *pTraderApi);
void ShowCommonInfor(CThostFtdcTraderApi *pTraderApi);
void dealOrder_SA_TEST(CTraderApi *pTradeApi);


#include<string>

namespace utils
{
    void creatFolder(const std::string folderName);
    void creatFile(const std::string fileName);

}


#endif
