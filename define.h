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
    CLIENT_NAME_ID            = 1,
    STRATEGY_ORDER_ID         = 2,
    MARKET_REQ_INSTRUMENT_ID  = 3,
    TRADE_INSTRUMENT_ID       = 4,
    GUI_REQ_FILEZ_ID          = 5,
    GUI_REQ_TICKZ_ID          = 6,
    STRATEGY_FILEZ_ID         = 7,
    STRATEGY_TICKZ_ID         = 8,
    TRADE_ORDER_STATUS_ID     = 9,
    ROUTE_LODIN_STATUS_ID     = 10,
    MARKET_DEEP_DATA_ID       = 11,
    CLIENT_HEARTBEAT_ID       = 12, /*客户端发送给路由端的心跳数据*/
    ROUTE_RESPONDS_ID         = 13 /*路由端回应心跳数据给客户端*/
};

constexpr char ROUTE_HEADBEAT_TIMER[]     = "route_headbeat_check";
constexpr U32  HEADBEAT_TIME_OUT_LENGTH  = 45 * 1000;
constexpr U32  HEADBEAT_CHECK_PERIOD     = 15 * 1000;

constexpr U32 FORCE_LOGOUT_TIME_OUT      = 10 * 1000;
constexpr char FORCE_LOG_OUT_TIMER[]     = "forceLogOut";

#endif


