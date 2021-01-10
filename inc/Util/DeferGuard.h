#pragma once

#include <functional>

namespace VanUtil {

class DeferGuard {
public:
    DeferGuard(std::function<void(void *)> cb, void *opaque = nullptr) : cb_(cb), opaque_(opaque)
    {
    }

    ~DeferGuard()
    {
        cb_(opaque_);
    }

private:
    std::function<void(void *)> cb_;
    void *opaque_;
};

}; // namespace VanUtil
