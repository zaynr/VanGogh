#pragma once

#include "OglAdapter/BasicData.h"

#include <string>
#include <cstdint>
#include <map>

namespace VanOGL {

enum TextureType {
    TEXTURE_AO,
    TEXTURE_Albedo,
    TEXTURE_Diffuse,
    TEXTURE_Height,
    TEXTURE_Metallic,
    TEXTURE_Normal,
    TEXTURE_Specular,

    TEXTURE_BUTT,
};

class TextureMgr {
public:
    static TextureMgr &instance();

    ~TextureMgr();

    int32_t loadTextures();

    GLuint getTexture(const std::string &path);

    GLuint load(const std::string &path, const std::string &type);

    GLuint load(const std::string &path, const TextureType type);

    std::string texTypeDic(const TextureType type);

private:
    TextureMgr();

private:
    std::map<std::string, Texture> textureMap_;
    std::string dic_[TEXTURE_BUTT];
};

}; // namespace VanOGL