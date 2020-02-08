#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H
#include<string>

bool GetHostInfo(std::string& hostName, std::string& Ip);
bool get_local_mac(std::string& macStr);
//std::string get_local_mac();
#endif
