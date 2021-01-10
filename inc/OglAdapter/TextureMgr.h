#pragma once
#include <glad/glad.h>

#include <string>
#include <cstdint>
#include <map>

namespace VanOGL {

enum class TextureType {
    AO,
    Albedo,
    Diffuse,
    Height,
    Metallic,
    Normal,
    Specular,
};

class TextureMgr {
public:
    struct TextureTraits {
        GLuint id_{0};
        TextureType type;
    };

public:
    static TextureMgr &instance();

    ~TextureMgr();

    GLuint load(const std::string &path, const TextureType type);

    static std::string texTypeDic(const TextureType type);

private:
    TextureMgr();

private:
    std::map<std::string, TextureTraits> textureMap_;
};

}; // namespace VanOGL