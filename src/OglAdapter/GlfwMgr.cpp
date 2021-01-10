#include "OglAdapter/GlfwMgr.h"
#include <iostream>

using namespace VanOGL;

GlfwMgr::GlfwMgr() : window_(nullptr)
{
}

GlfwMgr::~GlfwMgr()
{
}

GlfwMgr &GlfwMgr::instance()
{
    static GlfwMgr ins;
    return ins;
}

int GlfwMgr::init(const char *name, uint32_t width, uint32_t height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window_ = glfwCreateWindow(width, height, name, nullptr, nullptr);
    if (window_ == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    return 0;
}

void GlfwMgr::setTermFlag(int32_t GL_KEY)
{
    regisEvent({[&](void *data) {
                    auto window = reinterpret_cast<GLFWwindow *>(data);
                    glfwSetWindowShouldClose(window, true);
                },
                this->window_,
                GL_KEY,
                GLFW_PRESS});
}

void GlfwMgr::loopEv()
{
    for (auto &ev : pendingEv_) {
        if (glfwGetKey(window_, ev.key) == ev.evtype) {
            ev.cb(ev.opaque);
        }
    }
}

void GlfwMgr::run(const std::function<void()> &fn)
{
    if (window_ == nullptr) {
        exit(1);
    }

    while (!glfwWindowShouldClose(window_)) {
        loopEv();
        fn();
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    glfwTerminate();
}

void GlfwMgr::setInputMode(int32_t GL_MODE)
{
    glfwSetInputMode(window_, GLFW_CURSOR, GL_MODE);
}

void GlfwMgr::setCallback(glfwPosCb mouseCb, glfwPosCb scrollCb, glfwFbCb fbCb)
{
    glfwSetFramebufferSizeCallback(window_, fbCb);
    glfwSetCursorPosCallback(window_, mouseCb);
    glfwSetScrollCallback(window_, scrollCb);
}

void GlfwMgr::regisEvent(RegisEvent &&ev)
{
    pendingEv_.emplace_back(ev);
}
