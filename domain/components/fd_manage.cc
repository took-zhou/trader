#include "trader/domain/components/fd_manage.h"
#include "common/extern/log/log.h"
#include "common/extern/sqlite3/sqlite3.h"
#include "common/self/file_util.h"
#include "common/self/utils.h"

FdManage::FdManage() {
  auto &json_cfg = utils::JsonConfig::GetInstance();
  trader_data_path = json_cfg.GetConfig("trader", "ControlParaFilePath").get<std::string>();

  std::string trader_path = trader_data_path + "/control.db";
  utils::CreatFile(trader_path);

  if (sqlite3_open(trader_path.c_str(), &trader_conn) != SQLITE_OK) {
    ERROR_LOG("Cannot open database: %s\n", sqlite3_errmsg(trader_conn));
    sqlite3_close(trader_conn);
  }
  sqlite3_exec(trader_conn, "PRAGMA synchronous = OFF; ", 0, 0, 0);
  sqlite3_busy_timeout(trader_conn, 10);  // 10ms

  char *error_msg = nullptr;
  if (sqlite3_exec(trader_conn, "BEGIN", 0, 0, &error_msg) != SQLITE_OK) {
    ERROR_LOG("Sql error %s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(trader_conn);
  }
}

void FdManage::OpenThingsUp(void) {
  char *error_msg = nullptr;
  if (sqlite3_exec(trader_conn, "COMMIT", 0, 0, &error_msg) != SQLITE_OK) {
    ERROR_LOG("Sql error %s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(trader_conn);
  }
  if (sqlite3_exec(trader_conn, "BEGIN", 0, 0, &error_msg) != SQLITE_OK) {
    ERROR_LOG("Sql error %s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(trader_conn);
  }
}
