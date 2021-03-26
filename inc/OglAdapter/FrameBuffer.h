#pragma once

#include <glad/glad.h>
#include <map>
#include <cstdint>

namespace VanOGL {

class FrameBuffer {
public:
    FrameBuffer(GLuint width, GLuint height, GLuint colorIndex = 1);

    ~FrameBuffer();

    void configureDepthOnly();

    void configureNormal();

    void configureNormalTex();

    GLuint getID();

    GLuint getBuff(uint64_t index);

    void activate();

    void deactivate();

    bool checkValid();

private:
    GLuint genColorAtt();


private:
    GLuint width_;
    GLuint height_;
    bool configured_{false};
    // frame buffer
    GLuint fbo_{0};
    // GL_COLOR_ATTACHMENT0
    GLuint colorIndex_;
    // buffs
    std::map<uint64_t, GLuint> buffs_;
};

}; // namespace VanOGL
