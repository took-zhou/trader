#ifndef TRADER_ACCOUNT_STATUS_H
#define TRADER_ACCOUNT_STATUS_H
#include <memory>
#include <string>
#include <unordered_map>

struct AccountInfo {
  AccountInfo(const std::string &time, double balance, double available, uint64_t session_id, uint32_t ref_index)
      : update_time(time), balance(balance), available(available), session_id(session_id), ref_index(ref_index) {}
  std::string update_time;
  double balance;
  double available;
  uint64_t session_id;
  uint32_t ref_index;
};

struct AccountStatus {
 public:
  AccountStatus();
  void InitDatabase();
  void UpdateAccountStatus(const std::string &value1, double value2, double value3, uint64_t value4, const std::string &value5);
  void RemoveAccountStatus(const std::string &value1);
  void SelectAccountStatus(uint64_t *session_id, std::string *order_ref);
  void ReqAccountStatus(void);
  void RestoreFromSqlite3();
  std::unordered_map<std::string, std::shared_ptr<AccountInfo>> account_info_map;
};

#endif