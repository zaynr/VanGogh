#pragma once
#include "OglAdapter/Shader.h"
#include "OglAdapter/BasicData.h"

#include <memory>
#include <string>
#include <map>

namespace VanOGL {

class BaseShape {
public:
    BaseShape() = default;

    virtual ~BaseShape() = default;

    // virtual void inflateVAO();

    virtual void inflateVertex(uint32_t pos = 0);

    virtual void inflateNormal(uint32_t pos = 1);

    virtual void inflateTexture(uint32_t pos = 2);

    virtual void inflateIndex(uint32_t pos = 3);

    virtual void draw();

private:
    bool isIndexed_;
};

}; // namespace VanOGL
