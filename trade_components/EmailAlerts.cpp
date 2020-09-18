#include <iostream>

#include "Std_Types.h"
#include "EmailAlerts.h"
#include "CSmtp.h"
#include "json.h"
#include "log.h"
#include "getconfig.h"
#include "ThostFtdcUserApiStruct.h"
using json = nlohmann::json;
using namespace std;


#define test_tencent_ssl

CSmtp mail;

bool init_email(void)
{
    bool ret = true;
    try
    {
#if defined(test_gmail_tls)
        mail.SetSMTPServer("smtp.gmail.com", 587);
        mail.SetSecurityType(USE_TLS);
#elif defined(test_qq_ssl)
        mail.SetSMTPServer("smtp.qq.com", 465);
        mail.SetSecurityType(USE_SSL);
#elif defined(test_gmail_ssl)
        mail.SetSMTPServer("smtp.gmail.com", 465);
        mail.SetSecurityType(USE_SSL);
#elif defined(test_hotmail_TLS)
        mail.SetSMTPServer("smtp.live.com", 25);
        mail.SetSecurityType(USE_TLS);
#elif defined(test_aol_tls)
        mail.SetSMTPServer("smtp.aol.com", 587);
        mail.SetSecurityType(USE_TLS);
#elif defined(test_yahoo_ssl)
        mail.SetSMTPServer("plus.smtp.mail.yahoo.com", 465);
        mail.SetSecurityType(USE_SSL);
#elif defined(test_tencent_ssl)
        mail.SetSMTPServer("smtp.exmail.qq.com", 465);
        mail.SetSecurityType(USE_SSL);
#endif

        mail.SetLogin("zhoufan@cdsslh.com");
        mail.SetPassword("sCARbo12");
        mail.SetSenderName("huaxia");
        mail.SetSenderMail("zhoufan@cdsslh.com");
        //mail.SetReplyTo("zhoufan@cdsslh.com");
        mail.SetSubject("Huaxia strategy operation monitoring");
        mail.AddRecipient("zhoufan@cdsslh.com");
        mail.AddRecipient("zhanghaien@cdsslh.com");
        mail.AddRecipient("cuiwenhong@cdsslh.com");
        mail.SetXPriority(XPRIORITY_NORMAL);
        mail.SetXMailer("The Bat! (v3.02) Professional");
    }
    catch(ECSmtp e)
    {
        ERROR_LOG("Error: %s", e.GetErrorText().c_str());
        ret = false;
    }
    return ret;
}

bool send_email(const char* Text)
{
    bool ret = true;
    try
    {
        mail.AddMsgLine(Text);
        mail.AddMsgLine("");
        mail.AddMsgLine("Please do not reply to this system mail.");
        mail.Send();
    }
    catch(ECSmtp e)
    {
        ERROR_LOG("Error: %s", e.GetErrorText().c_str());
        ret = false;
    }
    return ret;
}

//bool send_email_order(ORDER_PARA *op)
//{
//    json order;
//
//    //浼犺緭鐨勬暟鎹祴鍊�
//    order["VolumeTotalOriginal1"] = op->Vol1;
//    order["VolumeTotalOriginal2"] = op->Vol2;
//
//    order["InstrumentID1"] = op->Contract1;
//    order["LimitPrice1"] = op->Price1;
//
//    order["InstrumentID2"] = op->Contract2;
//    order["LimitPrice2"] = op->Price2;
//
//    order["Direction"] = op->Bid;
//    order["ExchangeID"] = op->Exchange;
//    order["CombOffsetFlag"] =op->CombOffsetFlag;
//    order["limitPriceBuy"] =op->limitPriceBuy;
//    order["limitPriceSell"] =op->limitPriceSell;
//    order["priceUnit"] =op->priceUnit;
//
//    timeStr = getConfig("strategy", "UserID");
//
//    string msgbody = order.dump() + '\0';
//    return send_email(msgbody.c_str());
//}

bool send_email_order_insert(CThostFtdcInputOrderField& order1, CThostFtdcInputOrderField& order2, bool result)
{
    json order1Json;
    json order2Json;
    order1Json["ExchangeID"] = order1.ExchangeID;
    order1Json["InstrumentID"] = order1.InstrumentID;
    order1Json["Direction"] = order1.Direction == '0'? "buy" : "sell";
    order1Json["BrokerID"] = order1.BrokerID;
//    order1Json["InvestorID"] = order1.InvestorID;
//    order1Json["UserID"] = order1.UserID;
//    order1Json["AccountID"] = order1.AccountID;
//    order1Json["ContingentCondition"] = order1.ContingentCondition;
//    order1Json["CurrencyID"] = order1.CurrencyID;
//    order1Json["ForceCloseReason"] = order1.ForceCloseReason;
//    order1Json["GTDDate"] = order1.GTDDate;
//    order1Json["IPAddress"] = order1.IPAddress;
//    order1Json["InvestUnitID"] = order1.InvestUnitID;
//    order1Json["IsAutoSuspend"] = order1.IsAutoSuspend;
//    order1Json["IsSwapOrder"] = order1.IsSwapOrder;
//    order1Json["LimitPrice"] = order1.LimitPrice;
//    order1Json["MacAddress"] = order1.MacAddress;
//    order1Json["OrderPriceType"] = order1.OrderPriceType;
//    order1Json["OrderRef"] = order1.OrderRef;
//    order1Json["RequestID"] = order1.RequestID;
//    order1Json["StopPrice"] = order1.StopPrice;
//    order1Json["TimeCondition"] = order1.TimeCondition;
//    order1Json["UserForceClose"] = order1.UserForceClose;
//    order1Json["VolumeCondition"] = order1.VolumeCondition;
    order1Json["VolumeTotalOriginal"] = order1.VolumeTotalOriginal;
//    order1Json["MinVolume"] = order1.MinVolume;

    order2Json["ExchangeID"] = order2.ExchangeID;
    order2Json["InstrumentID"] = order2.InstrumentID;
    order2Json["Direction"] = order2.Direction == '0'? "buy" : "sell";
//    order2Json["BrokerID"] = order2.BrokerID;
//    order2Json["InvestorID"] = order2.InvestorID;
//    order2Json["UserID"] = order2.UserID;
//    order2Json["AccountID"] = order2.AccountID;
//    order2Json["ContingentCondition"] = order2.ContingentCondition;
//    order2Json["CurrencyID"] = order2.CurrencyID;
//    order2Json["ForceCloseReason"] = order2.ForceCloseReason;
//    order2Json["GTDDate"] = order2.GTDDate;
//    order2Json["IPAddress"] = order2.IPAddress;
//    order2Json["InvestUnitID"] = order2.InvestUnitID;
//    order2Json["IsAutoSuspend"] = order2.IsAutoSuspend;
//    order2Json["IsSwapOrder"] = order2.IsSwapOrder;
//    order2Json["LimitPrice"] = order2.LimitPrice;
//    order2Json["MacAddress"] = order2.MacAddress;
//    order2Json["OrderPriceType"] = order2.OrderPriceType;
//    order2Json["OrderRef"] = order2.OrderRef;
//    order2Json["RequestID"] = order2.RequestID;
//    order2Json["StopPrice"] = order2.StopPrice;
//    order2Json["TimeCondition"] = order2.TimeCondition;
//    order2Json["UserForceClose"] = order2.UserForceClose;
//    order2Json["VolumeCondition"] = order2.VolumeCondition;
    order2Json["VolumeTotalOriginal"] = order2.VolumeTotalOriginal;
//    order2Json["MinVolume"] = order2.MinVolume;

    json tmpJson = {
            {"order1",order1Json},
            {"order2",order2Json}
    };
    tmpJson["result"] = "failed";
    if(result)
    {
        tmpJson["result"] = "success";
    }
    INFO_LOG("begin to build email msgbody");
    string msgbody = "";
    msgbody += "trader orders insert:\n";
    msgbody += "strategy name: huaxia\n";
    msgbody +=  tmpJson.dump(4) + "\0";
    INFO_LOG("begin to send email");

    return send_email(msgbody.c_str());
}
