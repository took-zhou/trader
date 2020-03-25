
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
            INFO_LOG("create file[%s] ok",folderName.c_str());
        }
        else
        {
            INFO_LOG("file[%s] existed",folderName.c_str());
        }
    }
}
