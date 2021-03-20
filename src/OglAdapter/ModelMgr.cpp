#include "OglAdapter/ModelMgr.h"
#include "Util/FileSystem.h"
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

using namespace VanOGL;
using namespace VanUtil;
using namespace rapidjson;
using namespace std;

ModelMgr &ModelMgr::instance()
{
    static ModelMgr ins;
    return ins;
}

int32_t ModelMgr::loadModels()
{
    auto jsonBody = FileSystem::readFileToStr("assets/config/engine.json");
    auto p = make_unique<Document>();
    p->Parse(jsonBody.c_str());
    auto &doc   = *p;
    auto &model = doc["models"];
    assert(model.IsArray());
    for (SizeType i = 0; i < model.Size(); i++) {
        auto name = model[i]["name"].GetString();
        auto path = model[i]["path"].GetString();
        auto inner = make_shared<Model>(FileSystem::getAbsPath(path));
        models_[name] = inner;
    }

    return 0;
}

std::shared_ptr<Model> ModelMgr::getModel(const std::string &name)
{
    auto iter = models_.find(name);
    if (iter == models_.end()) {
        return nullptr;
    }
    return iter->second;
}
