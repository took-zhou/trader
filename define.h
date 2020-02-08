#ifndef DEFINE_H
#define DEFINE_H
//#include <stdio.h>


#define LOGDEMO
// #define DEBUG
#define TRADE


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

////�ͻ��˵�����
//#define STRATEGYNAME    "strategy"
//#define MARKETNAME      "market"
//#define TRADENAME       "trade"
//#define ROUTENAME       "route"
//#define REMOTENAME      "remote"
//
////body��id��
//#define CLIENTNAMEID            0x0001  /*�ͻ�������*/
//#define STRATEGYORDERID         0x0002  /*���Զ��µ�*/
//#define MARKETREQINSTRUMENTID   0x0003  /*�г�������ǰ�ɽ��׺�Լ*/
//#define TRADEINSTRUMENTID       0x0004  /*���׶˷��Ϳɽ��׺�Լ*/
//#define REMOTEREQFILEZID        0x0005  /*Զ�̶�����filez*/
//#define STRATEGYTICKZID         0x0006  /*���Զ˷���tickz*/
//#define STRATEGYFILEZID         0x0007  /*���Զ˷���filez*/
//#define TRADEORDRESTATUSID      0x0008  /*���׶˷����µ�״̬*/
//#define ROUTELODINSTATUS        0x0009  /*·�ɶ˷��͵�½״̬*/

//�ͻ��˵�����
#define STRATEGYNAME    "strategy"
#define MARKETNAME      "market"
#define TRADENAME       "trade"
#define ROUTENAME       "route"
#define GUINAME         "gui"



//body��id��
enum struct ModuleName
{
    CLIENT_NAME_ID  =       1,  /*�ͻ�������*/
    STRATEGY_ORDER_ID,          /*���Զ��µ�*/
    MARKET_REQ_INSTRUMENT_ID,   /*�г�������ǰ�ɽ��׺�Լ*/
    TRADE_INSTRUMENT_ID,        /*���׶˷��Ϳɽ��׺�Լ*/
    GUI_REQ_FILEZ_ID,           /*ͼ�������filez*/
    GUI_REQ_TICKZ_ID,           /*ͼ�������tickz*/
    STRATEGY_FILEZ_ID,          /*���Զ˷���filez*/
    STRATEGY_TICKZ_ID,          /*���Զ˷���tickz*/
    TRADE_ORDER_STATUS_ID,      /*���׶˷����µ�״̬*/
    ROUTE_LODIN_STATUS_ID,      /*·�ɶ˷��͵�½״̬*/
    MARKET_DEEP_DATA_ID         /*�г��˷��ͺ�Լ�����������*/

};
#endif


