
#include "utils.h"

#include "log.h"
#include <unistd.h>

namespace utils
{
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
            std::string command = "touch " + fileName;-
            system(command.c_str());
            INFO_LOG("create file[%s] ok", fileName.c_str());
        }
        else
        {
            INFO_LOG("file[%s] existed", fileName.c_str());
        }
    }

}
