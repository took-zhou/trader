#ifndef __INFOSHOW__H
#define __INFOSHOW__H
#include "ctpClient.h"
#include "PrintCheck.h"
#include "login_part.h"
#include "dci/Role.h"
#include <string>
#include <vector>
//#include<atomic>
using namespace std;
struct InfoShow
{
    void infoShowTest(string command);
    bool cmdSelect(const vector<string> splitedStr);
    void singleInstrumentQuery(string& instrumentId, string& exchangeId);
    USE_ROLE(CtpClient);
    USE_ROLE(PintCheck);
    USE_ROLE(LogInPart);
};

struct InfoShowFlag
{
    bool SingleInstrumentQuery {false};
};


#endif
