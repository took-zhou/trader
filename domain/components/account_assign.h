#ifndef TRADER_ACCOUNT_STATUS_H
#define TRADER_ACCOUNT_STATUS_H
#include <memory>
#include <string>
#include <unordered_map>
#include "common/extern/sqlite3/sqlite3.h"
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
  ~AccountAssign();

  void UpdateAccountStatus(double value1, double value2, uint64_t value3, const std::string &value4);
  void UpdateOpenBlackList(const std::string &value, const std::string &ins, const std::string &index);
  void ReqAccountStatus(void);
  void RemoveAccountStatus(void);
  void HandleTraderOpen();
  void HandleTraderClose();

  bool init_database_flag = false;
  std::unordered_map<std::string, std::shared_ptr<AccountInfo>> account_info_map;

 private:
  void PrepareSqlSentence();
  void RestoreFromSqlite3();
  void InitDatabase();
  sqlite3_stmt *insert_account_ = nullptr;
  sqlite3_stmt *update_account_ = nullptr;
  sqlite3_stmt *delete_account_ = nullptr;
  const uint16_t sql_length_ = 512;
  uint16_t ref_base_ = 1000;
};

#endif