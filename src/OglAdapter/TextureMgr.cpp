#include "OglAdapter/TextureMgr.h"
#include "Util/DeferGuard.h"
#include "Util/FileSystem.h"

#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace VanOGL;
using namespace VanUtil;
using namespace rapidjson;
using namespace std;

TextureMgr::TextureMgr()
{
    dic_[TextureType::TEXTURE_AO]       = "texture_ao";
    dic_[TextureType::TEXTURE_Albedo]   = "texture_albedo";
    dic_[TextureType::TEXTURE_Diffuse]  = "texture_diffuse";
    dic_[TextureType::TEXTURE_Height]   = "texture_height";
    dic_[TextureType::TEXTURE_Metallic] = "texture_metallic";
    dic_[TextureType::TEXTURE_Normal]   = "texture_normal";
    dic_[TextureType::TEXTURE_Specular] = "texture_specular";
}

TextureMgr::~TextureMgr()
{
}

TextureMgr &TextureMgr::instance()
{
    static TextureMgr ins;
    return ins;
}

GLuint TextureMgr::load(const std::string &path, const TextureType type)
{
    return load(path, dic_[type]);
}

GLuint TextureMgr::load(const std::string &path, const std::string &type)
{
    auto iter = textureMap_.find(path);
    if (iter != textureMap_.end()) {
        return iter->second.id_;
    }

    GLuint id;
    glGenTextures(1, &id);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    DeferGuard freeBuf([&](void *) { stbi_image_free(data); });
    if (data != nullptr) {
        GLenum format;
        switch (nrComponents) {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;

            default:
                format = GL_RGB;
                break;
        }

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        return 0;
    }

    textureMap_.emplace(std::make_pair(path, Texture{id, type}));
    return id;
}

std::string TextureMgr::texTypeDic(const TextureType type)
{
    return dic_[type];
}

int32_t TextureMgr::loadTextures()
{
    auto jsonBody = FileSystem::readFileToStr("assets/config/engine.json");
    auto p        = make_unique<Document>();
    p->Parse(jsonBody.c_str());
    auto &doc     = *p;
    auto &texture = doc["textures"];
    assert(texture.IsArray());
    for (SizeType i = 0; i < texture.Size(); i++) {
        auto path = texture[i]["path"].GetString();
        auto type = texture[i]["type"].GetString();
        load(FileSystem::getAbsPath(path), type);
    }
    return 0;
}

GLuint TextureMgr::getTexture(const std::string &path)
{
    auto iter = textureMap_.find(FileSystem::getAbsPath(path.c_str()));
    if (iter == textureMap_.end()) {
        return 0;
    }
    return iter->second.id_;
}
