#ifndef TRADER_ACCOUNT_STATUS_H
#define TRADER_ACCOUNT_STATUS_H
#include <memory>
#include <string>
#include <unordered_map>
#include "common/self/utils.h"

struct AccountInfo {
  AccountInfo(double balance, double available, const std::string &user_id, uint32_t order_ref)
      : balance(balance), available(available), user_id(user_id), order_ref(order_ref) {}
  double balance;
  double available;
  std::string user_id;
  uint32_t order_ref;
};

struct AccountAssign {
 public:
  AccountAssign();
  void InitDatabase();
  void UpdateAccountStatus(double value1, double value2, uint64_t value3, const std::string &value4);
  void ReqAccountStatus(void);
  void RemoveAccountStatus(void);
  void BuildOrderContent(std::shared_ptr<utils::OrderContent> &content);
  void RestoreFromSqlite3();

 private:
  std::unordered_map<uint64_t, std::shared_ptr<AccountInfo>> account_info_map_;
  const double minimum_account_available_ = 10000.0;
};

#endif