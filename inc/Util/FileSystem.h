#pragma once

#include <string>

namespace VanUtil {

class FileSystem {
public:
    ~FileSystem();

    static FileSystem &instance();

    static std::string getAbsPath(const char *rtvPath);

    static std::string readFileToStr(const char *rtvPath);

private:
    FileSystem();

private:
    /* data */
};

}; // namespace VanUtil
