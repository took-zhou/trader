#ifndef TRADER_ACCOUNT_STATUS_H
#define TRADER_ACCOUNT_STATUS_H
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include "common/extern/sqlite3/sqlite3.h"

struct AccountInfo {
 public:
  AccountInfo(double balance, double available, uint64_t session_id, uint32_t order_ref)
      : balance_(balance), available_(available), session_id_(session_id), order_ref_(order_ref) {}
  void SetBalance(double balance) { balance_ = balance; }
  void SetAvailable(double available) { available_ = available; }
  void SetSessionId(uint64_t session_id) { session_id_ = session_id; }
  void InsertOpenBlacklist(const std::string &ins) { open_blacklist_.insert(ins); }
  const std::set<std::string> &GetOpenBlacklist() { return open_blacklist_; };
  void ClearOpenBlackList(void) { open_blacklist_.clear(); }
  uint64_t GetSessionId(void) { return session_id_; }
  double GetBalance(void) { return balance_; }
  double GetAvailable(void) { return available_; }
  bool NotOnBlacklist(const std::string &ins) { return open_blacklist_.find(ins) == open_blacklist_.end(); }
  uint32_t IncOrderRef(void) { return ++order_ref_; };

 private:
  double balance_;
  double available_;
  uint64_t session_id_;
  uint32_t order_ref_;
  std::set<std::string> open_blacklist_;
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
  std::unordered_map<std::string, std::unique_ptr<AccountInfo>> &GetAccountInfoMap(void);

 private:
  void PrepareSqlSentence();
  void RestoreFromSqlite3();
  void InitDatabase();
  std::unordered_map<std::string, std::unique_ptr<AccountInfo>> account_info_map_;
  sqlite3_stmt *insert_account_ = nullptr;
  sqlite3_stmt *update_account_ = nullptr;
  sqlite3_stmt *delete_account_ = nullptr;
  const uint16_t sql_length_ = 512;
  uint16_t ref_base_ = 1000;
};

#endif