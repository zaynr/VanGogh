#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <list>

namespace VanOGL {
using EvCb = std::function<void(void *)>;

struct RegisEvent {
    EvCb cb;
    void *opaque;
    int key;
    int evtype;
};

class GlfwMgr {
public:
    using glfwPosCb = void(GLFWwindow *, double, double);
    using glfwFbCb  = void(GLFWwindow *, int, int);

public:
    static GlfwMgr &instance();
    ~GlfwMgr();

    int init(const char *name, uint32_t width, uint32_t height);

    void run(const std::function<void()> &fn);

    void setInputMode(int32_t GL_MODE);

    void setCallback(glfwPosCb mouseCb, glfwPosCb scrollCb, glfwFbCb fbCb);

    void regisEvent(RegisEvent &&ev);

    void setTermFlag(int32_t GL_KEY);

private:
    GlfwMgr();

    void loopEv();

private:
    GLFWwindow *window_;
    std::list<RegisEvent> pendingEv_;
};
}; // namespace VanOGL