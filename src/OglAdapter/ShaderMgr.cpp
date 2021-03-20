#include "OglAdapter/ShaderMgr.h"
#include "Util/FileSystem.h"
#include <memory>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

using namespace VanOGL;
using namespace VanUtil;
using namespace rapidjson;
using namespace std;

ShaderMgr &ShaderMgr::instance()
{
    static ShaderMgr ins;
    return ins;
}

int32_t ShaderMgr::loadShaders()
{
    auto jsonBody = FileSystem::readFileToStr("assets/config/engine.json");
    auto p = make_unique<Document>();
    p->Parse(jsonBody.c_str());
    auto &doc   = *p;
    auto &shader = doc["shaders"];
    assert(shader.IsArray());
    for (SizeType i = 0; i < shader.Size(); i++) {
        auto name = shader[i]["name"].GetString();
        auto vs = shader[i]["vs"].GetString();
        auto fs = shader[i]["fs"].GetString();
        auto inner = make_shared<Shader>();
        inner->load(vs, fs);
        shaders_[name] = inner;
    }
    return 0;
}

std::shared_ptr<Shader> ShaderMgr::getShader(const std::string &name)
{
    auto iter = shaders_.find(name);
    if (iter == shaders_.end()) {
        return nullptr;
    }
    return iter->second;
}
