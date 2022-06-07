#include "trader/domain/components/settlementConfirm.h"
#include <semaphore.h>
#include <time.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include "common/extern/json/json.h"
#include "common/extern/log/log.h"
#include "common/self/fileUtil.h"
#include "common/self/semaphorePart.h"
#include "common/self/utils.h"
#include "trader/domain/components/ctpTradeApi/ctpTradeApi.h"
extern GlobalSem globalSem;

using json = nlohmann::json;
inline bool is_file_exists(const std::string &name) { return (access(name.c_str(), F_OK) != -1); }

namespace {
std::map<std::string, std::string> MonthMap = {
    {"Jan", "01"},     {"January ", "01"}, {"Feb", "02"},      {"February", "02"}, {"Mar", "03"},       {"March", "03"},
    {"Apr", "04"},     {"April", "04"},    {"May", "05"},      {"Jun", "06"},      {"June", "06"},      {"Jul", "07"},
    {"July", "07"},    {"Aug", "08"},      {"August", "08"},   {"Sept", "09"},     {"Septemper", "09"}, {"Oct", "10"},
    {"October", "10"}, {"Nov", "11"},      {"November", "11"}, {"Dec", "12"},      {"December", "12"},
};
bool removeSpecialStrInTheEnd(string &str, const char &pattern) {
  int string_len = str.length();
  if (str.at(string_len - 1) == pattern) {
    str = str.substr(0, string_len - 1);
  }
  return true;
}
vector<string> split(const string &str, const string &pattern) {
  vector<string> res;
  if (str == "") return res;
  string strs = str + pattern;
  size_t pos = strs.find(pattern);
  while (pos != strs.npos) {
    string temp = strs.substr(0, pos);
    res.push_back(temp);
    strs = strs.substr(pos + 1, strs.size());
    pos = strs.find(pattern);
  }
  return res;
}
string getDate() {
  time_t now_time = time(NULL);
  // local time
  tm *local_time = localtime(&now_time);
  vector<string> split_strings = split(asctime(local_time), string(" "));
  int size = split_strings.size();
  string year = split_strings[size - 1];
  if (!removeSpecialStrInTheEnd(year, '\n')) {
    printf("orror in removeSpecialStrInTheEnd\n");
  }
  string month = MonthMap[split_strings[1]];
  string day = split_strings[2];

  return year + month + day;
}
}  // namespace

bool SettlementConfirm::confirm(const std::string tradingDay) {
  if (!hasConfirmedToday(tradingDay)) {
    INFO_LOG("begin to confirm settlement today");
    if (confirmToday(tradingDay)) {
      INFO_LOG("confirmToday ok!");
      return true;
    }
    ERROR_LOG("confirm today error");
    return false;
  }
  return true;
}

bool SettlementConfirm::hasConfirmedToday(const std::string tradingDay) {
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string confirmFile = jsonCfg.getConfig("trader", "ConfirmRecordPath").get<std::string>();
  //    string confirmFile = getConfig("trade", "ConfirmRecordPath");
  if (is_file_exists(confirmFile)) {
    ifstream istrm(confirmFile, ios::binary);
    if (!istrm.is_open()) {
      ERROR_LOG("failed to open %s",
                confirmFile.c_str());  // @suppress("Invalid arguments")
    }
    json record;
    istrm >> record;
    istrm.close();
    JsonPrint(record);

    if (record["date"].get<std::string>() == tradingDay && record["value"].get<int>() == 1) {
      INFO_LOG("settlement today has been confirmed before!");
      return true;
    }
    return false;
  }
  utils::creatFile(confirmFile);
  return false;
}

bool SettlementConfirm::confirmToday(const std::string tradingDay) {
  json record;
  record["date"] = tradingDay;
  record["value"] = 0;
  auto &jsonCfg = utils::JsonConfig::getInstance();
  const std::string confirmFile = jsonCfg.getConfig("trader", "ConfirmRecordPath").get<std::string>();
  ofstream ostrm(confirmFile, ios::trunc);
  ostrm << setw(4) << record << endl;
  ostrm.close();
  ROLE(CtpTraderApi).traderApi->ReqSettlementInfoConfirm();
  std::string semName = "trader_ReqSettlementInfoConfirm";
  globalSem.waitSemBySemName(semName);
  globalSem.delOrderSem(semName);

  record["value"] = 1;
  ofstream ostrm_set_1(confirmFile, ios::trunc);
  ostrm_set_1 << setw(4) << record << endl;
  ostrm_set_1.close();

  return true;
}
