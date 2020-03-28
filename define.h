#ifndef DEFINE_H
#define DEFINE_H
// #include <stdio.h>


#define LOGDEMO
// #define DEBUG
#define TRADE


typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;
typedef unsigned long  U64;

typedef signed char    S8;
typedef signed short   S16;
typedef signed int     S32;
typedef signed long    S64;

enum RspState
{
    sucess = 0,
    failed = 1
};

// trade interface
struct TradeMsgHead
{
    unsigned short dataTypeId;   //strategy id
    char fromClientName[20];     // strategy name
    char toClientName[20];       // trade name
    unsigned short length;       // msg body length
};

// //锟酵伙拷锟剿碉拷锟斤拷锟斤拷
// #define STRATEGYNAME    "strategy"
// #define MARKETNAME      "market"
// #define TRADENAME       "trade"
// #define ROUTENAME       "route"
// #define REMOTENAME      "remote"
//
// //body锟斤拷id锟斤拷
// #define CLIENTNAMEID            0x0001
// #define STRATEGYORDERID         0x0002
// #define MARKETREQINSTRUMENTID   0x0003
// #define TRADEINSTRUMENTID       0x0004
// #define REMOTEREQFILEZID        0x0005
// #define STRATEGYTICKZID         0x0006
// #define STRATEGYFILEZID         0x0007
// #define TRADEORDRESTATUSID      0x0008
// #define ROUTELODINSTATUS        0x0009

// 锟酵伙拷锟剿碉拷锟斤拷锟斤拷
#define STRATEGYNAME    "strategy"
#define MARKETNAME      "market"
#define TRADENAME       "trade"
#define ROUTENAME       "route"
#define GUINAME         "gui"


// body锟斤拷id锟斤拷
enum struct ModuleName
{
    CLIENT_NAME_ID  =       1,
    STRATEGY_ORDER_ID,
    MARKET_REQ_INSTRUMENT_ID,
    TRADE_INSTRUMENT_ID,
    GUI_REQ_FILEZ_ID,
    GUI_REQ_TICKZ_ID,
    STRATEGY_FILEZ_ID,
    STRATEGY_TICKZ_ID,
    TRADE_ORDER_STATUS_ID,
    ROUTE_LODIN_STATUS_ID,
    MARKET_DEEP_DATA_ID
};

constexpr char ROUTE_HEADBEAT_TIMER[]   = "route_headbeat_check";
constexpr U32  HEADBEAT_TIME_OUT_LENGTH = 80 * 1000;

#endif


