#ifndef TRADER_FD_MANAGE_H
#define TRADER_FD_MANAGE_H
#include <string>
#include "common/extern/sqlite3/sqlite3.h"

struct FdManage {
 public:
  FdManage();
  ~FdManage();
  FdManage(const FdManage &) = delete;
  FdManage &operator=(const FdManage &) = delete;
  static FdManage &GetInstance() {
    static FdManage instance;
    return instance;
  }

  void OpenThingsUp(void);
  sqlite3 *GetTraderConn();

 private:
  sqlite3 *trader_conn_ = NULL;
  std::string trader_data_path_ = "";
};

#endif