#pragma once
#include <string>
#include<vector>
#include<unordered_map>
using namespace std;

void getConfig_(string title, string cfgName,char* target);
string getConfig(string title, string cfgName);
string getConfigFromFile(const string configFile, string title, string cfgName);
std::vector<std::string> getAllTitles();
std::vector<std::string> getAllTitlesFromFile(const string configFile);

std::vector<std::pair<std::string,std::string>> getAllKeyValueOfTitleFromFile(const std::string configFile, std::string title);
std::vector<std::pair<std::string,std::string>> getAllKeyValueOfTitle(std::string title);
