#include "trader/domain/components/account_assign.h"
#include <unistd.h>
#include <cstring>
#include <thread>
#include "common/self/file_util.h"
#include "common/self/protobuf/ipc.pb.h"
#include "trader/domain/components/fd_manage.h"
#include "trader/domain/trader_service.h"
#include "trader/infra/recer_sender.h"

AccountAssign::AccountAssign() {
  init_database_flag = false;
  InitDatabase();
  PrepareSqlSentence();
  RestoreFromSqlite3();
}

AccountAssign::~AccountAssign() {
  sqlite3_finalize(insert_account_);
  sqlite3_finalize(update_account_);
  sqlite3_finalize(delete_account_);
}

void AccountAssign::InitDatabase() {
  if (init_database_flag == false) {
    char *error_msg = nullptr;
    const char *sql =
        "create table if not exists account_info(user_id TEXT, session_id INT, balance REAL, available REAL, open_blacklist TEXT);";
    if (sqlite3_exec(FdManage::GetInstance().trader_conn, sql, NULL, NULL, &error_msg) != SQLITE_OK) {
      ERROR_LOG("Sql error %s.", error_msg);
      sqlite3_free(error_msg);
      sqlite3_close(FdManage::GetInstance().trader_conn);
    }

    sql = "create table if not exists account(date TEXT, user_id TEXT, balance REAL);";
    if (sqlite3_exec(FdManage::GetInstance().trader_conn, sql, NULL, NULL, &error_msg) != SQLITE_OK) {
      ERROR_LOG("Sql error %s.", error_msg);
      sqlite3_free(error_msg);
      sqlite3_close(FdManage::GetInstance().trader_conn);
    }

    init_database_flag = true;
  }

  init_database_flag = true;
}

void AccountAssign::PrepareSqlSentence() {
  const char *sql =
      "insert into account_info(user_id, session_id, balance, available, open_blacklist) select ?, ?, ?, ?, '' where not exists (select * "
      "from account_info where user_id=?);";
  if (sqlite3_prepare_v2(FdManage::GetInstance().trader_conn, sql, strlen(sql), &insert_account_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().trader_conn);
  }
  sql = "update account_info set session_id = ?, balance = ?, available = ?, open_blacklist = ? where user_id = ?;";
  if (sqlite3_prepare_v2(FdManage::GetInstance().trader_conn, sql, strlen(sql), &update_account_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().trader_conn);
  }
  sql = "delete from account_info where user_id = ?;";
  if (sqlite3_prepare_v2(FdManage::GetInstance().trader_conn, sql, strlen(sql), &delete_account_, 0) != SQLITE_OK) {
    ERROR_LOG("prepare sql sentence error.");
    sqlite3_close(FdManage::GetInstance().trader_conn);
  }
}

void AccountAssign::UpdateAccountStatus(double value1, double value2, uint64_t value3, const std::string &value4) {
  if (account_info_map.find(value4) != account_info_map.end()) {
    std::string black_list;
    for (auto &item : account_info_map[value4]->open_blacklist) {
      black_list += item;
      black_list += ".";
    }
    account_info_map[value4]->balance = value1;
    account_info_map[value4]->available = value2;
    account_info_map[value4]->session_id = value3;
    sqlite3_reset(update_account_);
    sqlite3_bind_int64(update_account_, 1, value3);
    sqlite3_bind_double(update_account_, 2, value1);
    sqlite3_bind_double(update_account_, 3, value2);
    sqlite3_bind_text(update_account_, 4, black_list.c_str(), black_list.size(), 0);
    sqlite3_bind_text(update_account_, 5, value4.c_str(), value4.size(), 0);
    if (sqlite3_step(update_account_) != SQLITE_DONE) {
      ERROR_LOG("do sql sentence error.");
      sqlite3_close(FdManage::GetInstance().trader_conn);
    }
  }
}

void AccountAssign::UpdateOpenBlackList(const std::string &value, const std::string &ins, const std::string &index) {
  std::string temp_key;
  temp_key += ins;
  temp_key += ".";
  temp_key += index;
  if (account_info_map.find(value) != account_info_map.end()) {
    account_info_map[value]->open_blacklist.insert(temp_key);
  }
}

void AccountAssign::RemoveAccountStatus() { account_info_map.clear(); }

void AccountAssign::ReqAccountStatus(void) {
  auto &trader_ser = TraderService::GetInstance();
  auto &recer_sender = RecerSender::GetInstance();
  if (trader_ser.login_state == kLoginState) {
    ipc::message send_message;
    auto *accound_status_req = send_message.mutable_account_status_req();
    accound_status_req->set_indication(ipc::AccountStatusReq::start);

    utils::ItpMsg itp_msg;
    send_message.SerializeToString(&itp_msg.pb_msg);
    itp_msg.session_name = "trader_trader";
    itp_msg.msg_name = "AccountStatusReq";
    // 因为查询接口存在1s1次的限制，所以只能走ProxySender的接口
    recer_sender.ROLE(Sender).ROLE(ProxySender).SendMsg(itp_msg);
  }
}

void AccountAssign::HandleTraderOpen() {
  auto &json_cfg = utils::JsonConfig::GetInstance();
  auto users = json_cfg.GetConfig("trader", "User");
  std::set<std::string> user_id_set;
  for (auto &user : users) {
    const std::string user_id = json_cfg.GetDeepConfig("users", (std::string)user, "UserID").get<std::string>();
    user_id_set.insert(user_id);
  }
  std::vector<std::string> del_account_list;
  std::vector<std::string> add_account_list;
  for (auto &item : account_info_map) {
    if (user_id_set.find(item.first) == user_id_set.end()) {
      del_account_list.push_back(item.first);
    }
  }
  for (auto &item : user_id_set) {
    if (account_info_map.find(item) == account_info_map.end()) {
      add_account_list.push_back(item);
    }
  }
  if (del_account_list.size() == 0 && add_account_list.size() == 0) {
    INFO_LOG("no need to change account info.");
  }
  for (auto &item : del_account_list) {
    account_info_map.erase(item);
    INFO_LOG("del account: %s", item.c_str());
  }
  for (auto &item : add_account_list) {
    uint32_t new_order_ref = (uint32_t)((--ref_base_ % 1000) * 1000000);
    account_info_map[item] = std::make_shared<AccountInfo>(0.0, 0.0, 0, new_order_ref);
    sqlite3_reset(insert_account_);
    sqlite3_bind_text(insert_account_, 1, item.c_str(), item.size(), 0);
    sqlite3_bind_int64(insert_account_, 2, 0);
    sqlite3_bind_double(insert_account_, 3, 0);
    sqlite3_bind_double(insert_account_, 4, 0);
    sqlite3_bind_text(insert_account_, 5, item.c_str(), item.size(), 0);
    if (sqlite3_step(insert_account_) != SQLITE_DONE) {
      ERROR_LOG("do sql sentence error.");
      sqlite3_close(FdManage::GetInstance().trader_conn);
    }
  }
}

void AccountAssign::HandleTraderClose() {
  auto &trader_ser = TraderService::GetInstance();
  char sql[sql_length_];
  char *error_msg = nullptr;
  snprintf(sql, sql_length_, "delete from account where date = '%s';", trader_ser.ROLE(HandleState).trder_date.c_str());
  if (sqlite3_exec(FdManage::GetInstance().trader_conn, sql, NULL, NULL, &error_msg) != SQLITE_OK) {
    ERROR_LOG("Sql error %s.", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(FdManage::GetInstance().trader_conn);
  }

  for (auto &item : account_info_map) {
    item.second->open_blacklist.clear();
    if (item.second->session_id != 0) {
      snprintf(sql, sql_length_, "insert into account values ('%s', '%s', %f);", trader_ser.ROLE(HandleState).trder_date.c_str(),
               item.first.c_str(), item.second->balance);
      if (sqlite3_exec(FdManage::GetInstance().trader_conn, sql, NULL, NULL, &error_msg) != SQLITE_OK) {
        ERROR_LOG("Sql error %s.", error_msg);
        sqlite3_free(error_msg);
        sqlite3_close(FdManage::GetInstance().trader_conn);
      }
    }
  }
}

void AccountAssign::RestoreFromSqlite3() {}