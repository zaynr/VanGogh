#pragma once
#include "OglAdapter/Model.h"
#include <map>
#include <string>
#include <memory>

namespace VanOGL {

class ModelMgr {
public:
    ~ModelMgr() = default;

    static ModelMgr &instance();

    int32_t loadModels();

    std::shared_ptr<Model> getModel(const std::string &name);

private:
    ModelMgr() = default;

private:
    std::map<std::string, std::shared_ptr<Model>> models_;
};

}; // namespace VanOGL
