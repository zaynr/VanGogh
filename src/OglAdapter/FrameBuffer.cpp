#include "OglAdapter/FrameBuffer.h"
#include <iostream>

using namespace VanOGL;

FrameBuffer::FrameBuffer(GLuint width, GLuint height, GLuint colorIndex)
    : width_(width), height_(height), colorIndex_(colorIndex)
{
    glGenFramebuffers(1, &fbo_);
}

FrameBuffer::~FrameBuffer()
{
}

GLuint FrameBuffer::genColorAtt()
{
    GLuint tbo;
    glGenTextures(1, &tbo);
    glBindTexture(GL_TEXTURE_2D, tbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return tbo;
}

void FrameBuffer::configureDepthOnly()
{
    if (configured_) {
        return;
    }
    configured_ = true;

    activate();

    GLuint tbo;
    glGenTextures(1, &tbo);
    glBindTexture(GL_TEXTURE_2D, tbo);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width_, height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tbo, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    buffs_[GL_DEPTH_ATTACHMENT] = tbo;

    deactivate();

    checkValid();
}

void FrameBuffer::configureNormal()
{
    if (configured_) {
        return;
    }
    configured_ = true;

    activate();

    // generate texture
    for (GLuint i = 0; i < colorIndex_; ++i) {
        auto tbo = genColorAtt();
        auto index = GL_COLOR_ATTACHMENT0 + i;
        glFramebufferTexture2D(GL_FRAMEBUFFER, index, GL_TEXTURE_2D, tbo, 0);
        buffs_[index] = tbo;
    }

    // gen render buff
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    deactivate();

    checkValid();
}

void FrameBuffer::configureNormalTex()
{
    if (configured_) {
        return;
    }
    configured_ = true;

    activate();

    // generate texture
    for (GLuint i = 0; i < colorIndex_; ++i) {
        auto tbo = genColorAtt();
        auto index = GL_COLOR_ATTACHMENT0 + i;
        glFramebufferTexture2D(GL_FRAMEBUFFER, index, GL_TEXTURE_2D, tbo, 0);
        buffs_[index] = tbo;
    }

    // d and s
    GLuint tbo;
    glGenTextures(1, &tbo);
    glBindTexture(GL_TEXTURE_2D, tbo);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width_, height_, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tbo, 0);
    buffs_[GL_DEPTH_STENCIL_ATTACHMENT] = tbo;

    deactivate();

    checkValid();
}

GLuint FrameBuffer::getID()
{
    return fbo_;
}

void FrameBuffer::activate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void FrameBuffer::deactivate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBuffer::checkValid()
{
    if (!configured_) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not configured!" << std::endl;
        return false;
    }
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        return false;
    }
    std::cout << "INFO::FRAMEBUFFER:: [" << fbo_ << "] Framebuffer is complete!" << std::endl;
    return true;
}

GLuint FrameBuffer::getBuff(uint64_t index)
{
    return buffs_[index];
}
