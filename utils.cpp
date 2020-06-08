
#include "utils.h"

#include "log.h"
#include <unistd.h>
#include<string>
#include<vector>
namespace utils
{
    vector<string> splitString(const string& str, const string& pattern)
    {
        vector<string> res;
        if (str == "")
            return res;
        string strs = str + pattern;
        size_t pos = strs.find(pattern);
        while (pos != strs.npos)
        {
            string temp = strs.substr(0, pos);
            res.push_back(temp);
            strs = strs.substr(pos + 1, strs.size());
            pos = strs.find(pattern);
        }
        return res;
    }
    std::vector<std::string> splitDirAndFileName(const std::string str)
    {
        std::vector<std::string> dirAndFile;
        if (str == "")
        {
            return dirAndFile;
        }
        bool isAbsolutePath{ false };
        if (str[0] == '/')
        {
            isAbsolutePath = true;
        }
        auto splitedStr = splitString(str, "/");
        if (splitedStr.size() < 1)
        {
            ERROR_LOG("%s","input dir and file name str is error");
            return dirAndFile;
        }
        if (splitedStr.size() == 1)
        {
            dirAndFile.emplace_back(std::string(""));
            dirAndFile.emplace_back(splitedStr[0]);
            return dirAndFile;
        }
        auto vec_size = splitedStr.size();
        std::string tmpDir = "";
        if (isAbsolutePath)
        {
            tmpDir = "/";
        }
        for (decltype(vec_size) i = 0; i < vec_size - 1; ++i)
        {   
            if (splitedStr[i] == "" || splitedStr[i] == " ")
            {
                continue;
            }
            tmpDir = tmpDir+(splitedStr[i] + "/");
        }
        dirAndFile.emplace_back(tmpDir);
        dirAndFile.emplace_back(splitedStr[vec_size - 1]);
        return dirAndFile;
    }
    void creatFolder(const std::string folderName)
    {
        if (access(folderName.c_str(), 0) == -1)
        {
            std::string command = "mkdir -p " + folderName;
            system(command.c_str());
            INFO_LOG("create dir[%s] ok",folderName.c_str());
        }
        else
        {
            INFO_LOG("dir[%s] existed",folderName.c_str());
        }
    }



    void creatFile(const std::string fileName)
    {
        if (access(fileName.c_str(), 0) == -1)
        {
            std::vector<std::string> dirAndFile = splitDirAndFileName(fileName);
            if (dirAndFile[0] != "")
            {
                creatFolder(dirAndFile[0]);
            }
            std::string command = "touch " + fileName;
            system(command.c_str());
            INFO_LOG("create file[%s] ok", fileName.c_str());
        }
        else
        {
            INFO_LOG("file[%s] existed", fileName.c_str());
        }
    }

}
