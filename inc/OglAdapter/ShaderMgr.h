#pragma once
#include "OglAdapter/Shader.h"
#include <memory>
#include <string>
#include <map>

namespace VanOGL {

class ShaderMgr {
public:
    ~ShaderMgr() = default;

    static ShaderMgr &instance();

    int32_t loadShaders();

    std::shared_ptr<Shader> getShader(const std::string &name);

private:
    ShaderMgr() = default;

private:
    std::map<std::string, std::shared_ptr<Shader>> shaders_;
};

}; // namespace VanOGL
