#pragma once

#include <cstdint>
#include <memory>
#include <atomic>

namespace VanUtil {

// TODO: multi threading
class GlobalConfig {
public:
    ~GlobalConfig();

    static GlobalConfig &instance();

    void setScreenWidth(const int32_t in)
    {
        screenWidth_ = in;
    }

    int32_t getScreenWidth() const
    {
        return screenWidth_;
    }

    void setScreenHeight(const int32_t in)
    {
        screenHeight_ = in;
    }

    int32_t getScreenHeight() const
    {
        return screenHeight_;
    }

private:
    GlobalConfig();

private:
    int32_t screenWidth_;
    int32_t screenHeight_;
};

}; // namespace VanUtil
