#ifndef TRADER_ACCOUNT_STATUS_H
#define TRADER_ACCOUNT_STATUS_H
#include <memory>
#include <string>
#include <unordered_map>
#include "common/self/utils.h"

struct AccountInfo {
  AccountInfo(double balance, double available, uint64_t session_id, uint32_t order_ref)
      : balance(balance), available(available), session_id(session_id), order_ref(order_ref) {}
  double balance;
  double available;
  uint64_t session_id;
  uint32_t order_ref;
  std::set<std::string> open_blacklist;
};

struct AccountAssign {
 public:
  AccountAssign();
  void InitDatabase();
  void UpdateAccountStatus(double value1, double value2, uint64_t value3, const std::string &value4);
  void UpdateOpenBlackList(const std::string &value, const std::string &ins, const std::string &index);
  void ReqAccountStatus(void);
  void RemoveAccountStatus(void);
  void RestoreFromSqlite3();

  std::unordered_map<std::string, std::shared_ptr<AccountInfo>> account_info_map;
};

#endif