#ifndef DEFINE_H
#define DEFINE_H
//#include <stdio.h>


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

////锟酵伙拷锟剿碉拷锟斤拷锟斤拷
//#define STRATEGYNAME    "strategy"
//#define MARKETNAME      "market"
//#define TRADENAME       "trade"
//#define ROUTENAME       "route"
//#define REMOTENAME      "remote"
//
////body锟斤拷id锟斤拷
//#define CLIENTNAMEID            0x0001  /*锟酵伙拷锟斤拷锟斤拷锟斤拷*/
//#define STRATEGYORDERID         0x0002  /*锟斤拷锟皆讹拷锟铰碉拷*/
//#define MARKETREQINSTRUMENTID   0x0003  /*锟叫筹拷锟斤拷锟斤拷锟斤拷前锟缴斤拷锟阶猴拷约*/
//#define TRADEINSTRUMENTID       0x0004  /*锟斤拷锟阶端凤拷锟酵可斤拷锟阶猴拷约*/
//#define REMOTEREQFILEZID        0x0005  /*远锟教讹拷锟斤拷锟斤拷filez*/
//#define STRATEGYTICKZID         0x0006  /*锟斤拷锟皆端凤拷锟斤拷tickz*/
//#define STRATEGYFILEZID         0x0007  /*锟斤拷锟皆端凤拷锟斤拷filez*/
//#define TRADEORDRESTATUSID      0x0008  /*锟斤拷锟阶端凤拷锟斤拷锟铰碉拷状态*/
//#define ROUTELODINSTATUS        0x0009  /*路锟缴端凤拷锟酵碉拷陆状态*/

//锟酵伙拷锟剿碉拷锟斤拷锟斤拷
#define STRATEGYNAME    "strategy"
#define MARKETNAME      "market"
#define TRADENAME       "trade"
#define ROUTENAME       "route"
#define GUINAME         "gui"



//body锟斤拷id锟斤拷
enum struct ModuleName
{
    CLIENT_NAME_ID  =       1,  /*锟酵伙拷锟斤拷锟斤拷锟斤拷*/
    STRATEGY_ORDER_ID,          /*锟斤拷锟皆讹拷锟铰碉拷*/
    MARKET_REQ_INSTRUMENT_ID,   /*锟叫筹拷锟斤拷锟斤拷锟斤拷前锟缴斤拷锟阶猴拷约*/
    TRADE_INSTRUMENT_ID,        /*锟斤拷锟阶端凤拷锟酵可斤拷锟阶猴拷约*/
    GUI_REQ_FILEZ_ID,           /*图锟斤拷锟斤拷锟斤拷锟絝ilez*/
    GUI_REQ_TICKZ_ID,           /*图锟斤拷锟斤拷锟斤拷锟絫ickz*/
    STRATEGY_FILEZ_ID,          /*锟斤拷锟皆端凤拷锟斤拷filez*/
    STRATEGY_TICKZ_ID,          /*锟斤拷锟皆端凤拷锟斤拷tickz*/
    TRADE_ORDER_STATUS_ID,      /*锟斤拷锟阶端凤拷锟斤拷锟铰碉拷状态*/
    ROUTE_LODIN_STATUS_ID,      /*路锟缴端凤拷锟酵碉拷陆状态*/
    MARKET_DEEP_DATA_ID         /*锟叫筹拷锟剿凤拷锟酵猴拷约锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�*/

};
#endif


