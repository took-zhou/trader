#ifndef REQ_ACCOUNT_STATUS_H
#define REQ_ACCOUNT_STATUS_H

struct PubAccountStatus {
 public:
  PubAccountStatus();
  void ReqCycle(void);

  ~PubAccountStatus(){};
};

#endif