#include "OglAdapter/TextureMgr.h"
#include "Util/DeferGuard.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace VanOGL;
using namespace VanUtil;

TextureMgr::TextureMgr()
{
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

    textureMap_.emplace(std::make_pair(path, TextureTraits{id, type}));
    return id;
}

std::string texTypeDic(const TextureType type)
{
    using dicType      = std::map<TextureType, std::string>;
    static dicType dic = {{TextureType::AO, "tex_ao"},
                          {TextureType::Albedo, "tex_albedo"},
                          {TextureType::Diffuse, "tex_diffuse"},
                          {TextureType::Height, "tex_height"},
                          {TextureType::Metallic, "tex_metallic"},
                          {TextureType::Normal, "tex_normal"},
                          {TextureType::Specular, "tex_specular"}};

    return dic[type];
}
