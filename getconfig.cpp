
// #include "stdafx.h"
#include "getconfig.h"
// #include <wtypes.h>
// #include <conio.h>
#include <iostream>
#include <locale>
#include <string>
#include <cstring>
#include <fstream>
// #include <conio.h>
#include <time.h>
#include <sstream>
#include <locale.h>
#include <vector>
#include <stdio.h>

/*函数名称：getConfig()
函数功能：获取配置文件ini中相应大标题title下指定配置字段cfgname的值
参数1：string title		大标题[***]
参数2：string cfgName		大标题下的配置字段
返回值：配置文件ini中相应大标题title下指定配置字段cfgname的值
*/
using namespace std;

string getConfigFromFile(const string configFile, string title, string cfgName);

void getConfig_(string title, string cfgName,char* target)
{
	const char* INIFile = "config.ini";
	ifstream inifile(INIFile);
	if (!inifile.is_open())
	{
		cerr << "Could not open " << INIFile << endl;
		inifile.clear();
		// _getch();
		getchar();
		exit(-1);
	}
	string strtmp, strtitle, strcfgname, returnValue;
	int flag = 0;
	while (getline(inifile, strtmp,'\n'))
	{	
		std::size_t found = strtmp.find_last_of("\r");     //去除'\r'
		if(found !=std::string::npos){
			strtmp[found]='\0';
		}
		
		if (strtmp.substr(0, 1) == "#")	continue;	//过滤注释
		if (flag == 0)
		{
			if (strtmp.find(title) != string::npos)
			{
				if (strtmp.substr(0, 1) == "[")
				{
					if (strtmp.find("]") == string::npos) 	break;	//缺失“]”退出
					strtitle = strtmp.substr(1);
					strtitle = strtitle.erase(strtitle.find("]"));
					if (strtitle == title)		//找到大标题设置标志位为1，这样就不再找下一个大标题了
					{
						flag = 1;
						continue;
					}
				}
			}
		}
		if (flag == 1)
		{
			if (strtmp.substr(0, 1) == "[")	break;	//如果遇到下一个[]号说明当前大标题对应的配置字段搜索完毕，结束搜索。
			if (strtmp.find(cfgName) != string::npos)
			{
				if (strtmp.find("=") == string::npos)	break;	//缺失“=”退出
				strcfgname = strtmp;
				strcfgname = strcfgname.erase(strcfgname.find("="));
				if (strcfgname == cfgName)		//找到大标题相应的字段后，返回值
				{
					// target = strtmp.substr(strtmp.find("=") + 1).c_str();
					strcpy(target,strtmp.substr(strtmp.find("=") + 1).c_str());
					
					printf("aaaa:%s\n",strtmp.substr(strtmp.find("=") + 1).c_str());
					// char[10] ty = (char[10])target;
					// for(int ii=0;ii<=10;ii++){
					// 	printf(target[ii],"\n");
					// }
					// std::cout << "hhaah "<<target << std::endl;
					// target = returnValue;
					return ;
				}
				else continue;
			}
		}
	}
	cout << "配置文件错误：没找到" << title << "对应配置项" << cfgName << "！" << endl;
	// _getch();
	getchar();
	exit(-1);
}

string getConfig(string title, string cfgName)
{
	const string INIFile = "../../project/projroot/config.ini";
	return getConfigFromFile(INIFile,title,cfgName);	
}

string getConfigFromFile(const string configFile, string title, string cfgName)
{	
	ifstream inifile(configFile.c_str());
	if (!inifile.is_open())
	{
		cerr << "Could not open " << configFile << endl;
		inifile.clear();
		// _getch();
		getchar();
		exit(-1);
	}
	string strtmp, strtitle, strcfgname, returnValue;
	int flag = 0;
	while (getline(inifile, strtmp, '\n'))
	{
		std::size_t found = strtmp.find_last_of("\r"); //去除'\r'
		if (found != std::string::npos) {
			strtmp[found] = '\0';
		}

		if (strtmp.substr(0, 1) == "#")	continue;	//过滤注释
		if (flag == 0)
		{
			if (strtmp.find(title) != string::npos)
			{
				if (strtmp.substr(0, 1) == "[")
				{
					if (strtmp.find("]") == string::npos) 	break;	//缺失“]”退出
					strtitle = strtmp.substr(1);
					strtitle = strtitle.erase(strtitle.find("]"));
					if (strtitle == title)		//找到大标题设置标志位为1，这样就不再找下一个大标题了
					{
						flag = 1;
						continue;
					}
				}
			}
		}
		if (flag == 1)
		{
			if (strtmp.substr(0, 1) == "[")	break;	//如果遇到下一个[]号说明当前大标题对应的配置字段搜索完毕，结束搜索。
			if (strtmp.find(cfgName) != string::npos)
			{
				if (strtmp.find("=") == string::npos)	break;	//缺失“=”退出
				strcfgname = strtmp;
				strcfgname = strcfgname.erase(strcfgname.find("="));
				if (strcfgname == cfgName)		//找到大标题相应的字段后，返回值
				{
					returnValue = strtmp.substr(strtmp.find("=") + 1);
					// std::cout << returnValue << std::endl;
					// target = returnValue;
					return returnValue;
				}
				else continue;
			}
		}
	}
	cout << "配置文件错误：没找到" << title << "对应配置项" << cfgName << "！" << endl;
}

std::vector<std::string> getAllTitles()
{
    const string INIFile = "../../project/projroot/config.ini";
    return getAllTitlesFromFile(INIFile);
}

std::vector<std::string> getAllTitlesFromFile(const string configFile)
{
    ifstream inifile(configFile.c_str());
    if (!inifile.is_open())
    {
        cerr << "Could not open " << configFile << endl;
        inifile.clear();
        // _getch();
        getchar();
        exit(-1);
    }
    std::vector<std::string> titles;
    string strtmp, strtitle, strcfgname, returnValue;
    while (getline(inifile, strtmp, '\n'))
    {
        std::size_t found = strtmp.find_last_of("\r"); //去除'\r'
        if (found != std::string::npos) {
            strtmp[found] = '\0';
        }

        if (strtmp.substr(0, 1) == "#") continue;   //过滤注释
        if(strtmp.substr(0,1) == "[")
        {
            std::size_t posTail = strtmp.find("]");
            if(posTail == std::string::npos)
            {
                continue;
            }
            titles.push_back(strtmp.substr(1,posTail-1));
        }
    }
    return titles;
}

namespace
{
    vector<string> split(const string &str, const string &pattern)
    {
        vector<string> res;
        if(str == "")
        {
            return res;
        }
        string strs = str + pattern;
        size_t pos = strs.find(pattern);
        while(pos != strs.npos)
        {
            string temp = strs.substr(0, pos);
            res.push_back(temp);
            strs = strs.substr(pos+1, strs.size());
            pos = strs.find(pattern);
        }
        return res;
    }
}

std::vector<std::pair<std::string,std::string>> getAllKeyValueOfTitle(std::string title)
{
    const string INIFile = "../../project/projroot/config.ini";
    return getAllKeyValueOfTitleFromFile(INIFile,title);
}
std::vector<std::pair<std::string,std::string>> getAllKeyValueOfTitleFromFile(const std::string configFile, std::string title)
{
    ifstream inifile(configFile.c_str());
    if (!inifile.is_open())
    {
        cerr << "Could not open " << configFile << endl;
        inifile.clear();
        // _getch();
        getchar();
        exit(-1);
    }
    std::vector<std::pair<std::string,std::string>> keyValueAarray;
    string strtmp, strtitle, strcfgname, returnValue;
    int flag = 0;
    while (getline(inifile, strtmp, '\n'))
    {
        std::size_t found = strtmp.find_last_of("\r"); //去除'\r'
        if (found != std::string::npos) {
            strtmp[found] = '\0';
        }

        if (strtmp.substr(0, 1) == "#") continue;   //过滤注释
        if (flag == 0)
        {
            if (strtmp.find(title) != string::npos)
            {
                if (strtmp.substr(0, 1) == "[")
                {
                    if (strtmp.find("]") == string::npos)   break;  //缺失“]”退出
                    strtitle = strtmp.substr(1);
                    strtitle = strtitle.erase(strtitle.find("]"));
                    if (strtitle == title)      //找到大标题设置标志位为1，这样就不再找下一个大标题了
                    {
                        flag = 1;
                        continue;
                    }
                }
            }
        }

        if (flag == 1)
        {
            if (strtmp.substr(0, 1) == "[")
            {
                flag = 0;
                continue;  //如果遇到下一个[]号说明当前大标题对应的配置字段搜索完毕，结束搜索。
            }

            if (strtmp.find("=") == std::string::npos)
            {
                continue;  //缺失“=”
            }
            auto splitedStr = split(strtmp,std::string("="));
            std::pair<std::string,std::string> tmpPair = {splitedStr.at(0),splitedStr.at(1)};
            keyValueAarray.push_back(tmpPair);
        }
    }
    return keyValueAarray;

}



