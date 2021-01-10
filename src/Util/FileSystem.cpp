#include "Util/FileSystem.h"
#include <root_directory.h>
#include <sstream>
#include <fstream>

using namespace VanUtil;

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

FileSystem &FileSystem::instance()
{
    static FileSystem ins;
    return ins;
}

std::string FileSystem::getAbsPath(const char *rtvPath)
{
    std::stringstream ss;
    ss << g_ProjectRoot << "/" << rtvPath;
    return ss.str();
}

std::string FileSystem::readFileToStr(const char *rtvPath)
{
    std::ifstream ifs(getAbsPath(rtvPath));
    if (!ifs.is_open()) {
        return "";
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}
