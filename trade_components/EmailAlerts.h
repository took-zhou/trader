#ifndef EMAILALERT_H
#define EMAILALERT_H

struct CThostFtdcInputOrderField;

bool init_email(void);
bool send_email(const char* Text);
//bool send_email_order(ORDER_PARA *op);
bool send_email_order_insert(CThostFtdcInputOrderField& order1, CThostFtdcInputOrderField& order2, bool result);

#endif

