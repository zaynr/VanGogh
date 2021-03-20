#include "OglAdapter/GlfwMgr.h"
#include "OglAdapter/Camera.h"
#include "OglAdapter/TextureMgr.h"
#include "OglAdapter/Shader.h"
#include "OglAdapter/ModelMgr.h"
#include "OglAdapter/ShaderMgr.h"
#include "Util/FileSystem.h"
#include "Util/GlobalConfig.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace VanOGL;
using namespace VanUtil;

// camera
Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    auto &gi = GlobalConfig::instance();
    gi.setScreenWidth(width);
    gi.setScreenHeight(height);
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static float lastX     = 0.0f;
    static float lastY     = 0.0f;
    if (firstMouse) {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    mainCamera.updateCameraFace(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    // mainCamera.ProcessMouseScroll(yoffset);
}

void setMouseHidden(void *)
{
    constexpr const time_t epoch = 1;
    static bool flip             = false;
    static time_t last           = 0;
    time_t now                   = time(nullptr);

    if (flip) {
        GlfwMgr::instance().setInputMode(GLFW_CURSOR_DISABLED);
    } else {
        GlfwMgr::instance().setInputMode(GLFW_CURSOR_NORMAL);
    }
    if (now - last > epoch) {
        last = now;
        flip = !flip;
    }
}

void prepare()
{
    auto &gi = GlobalConfig::instance();
    GlfwMgr::instance().init("VanGogh", gi.getScreenWidth(), gi.getScreenHeight());

    GlfwMgr::instance().setTermFlag(GLFW_KEY_ESCAPE);

    GlfwMgr::instance().regisEvent(
        {[&](void *) { mainCamera.updateCameraPos(CameraMovement::FORWARD); }, nullptr, GLFW_KEY_W, GLFW_PRESS});

    GlfwMgr::instance().regisEvent(
        {[&](void *) { mainCamera.updateCameraPos(CameraMovement::BACKWARD); }, nullptr, GLFW_KEY_S, GLFW_PRESS});

    GlfwMgr::instance().regisEvent(
        {[&](void *) { mainCamera.updateCameraPos(CameraMovement::LEFT); }, nullptr, GLFW_KEY_A, GLFW_PRESS});

    GlfwMgr::instance().regisEvent(
        {[&](void *) { mainCamera.updateCameraPos(CameraMovement::RIGHT); }, nullptr, GLFW_KEY_D, GLFW_PRESS});

    GlfwMgr::instance().regisEvent(
        {[&](void *) { mainCamera.updateCameraPos(CameraMovement::UP); }, nullptr, GLFW_KEY_Q, GLFW_PRESS});

    GlfwMgr::instance().regisEvent(
        {[&](void *) { mainCamera.updateCameraPos(CameraMovement::DOWN); }, nullptr, GLFW_KEY_E, GLFW_PRESS});

    GlfwMgr::instance().regisEvent({[&](void *) { mainCamera.speedUp(); }, nullptr, GLFW_KEY_UP, GLFW_PRESS});

    GlfwMgr::instance().regisEvent({[&](void *) { mainCamera.slowDown(); }, nullptr, GLFW_KEY_DOWN, GLFW_PRESS});

    GlfwMgr::instance().regisEvent({[&](void *) { mainCamera.switchProjection(); }, nullptr, GLFW_KEY_P, GLFW_PRESS});

    GlfwMgr::instance().regisEvent({setMouseHidden, nullptr, GLFW_KEY_H, GLFW_PRESS});

    GlfwMgr::instance().setInputMode(GLFW_CURSOR_DISABLED);

    GlfwMgr::instance().setCallback(mouse_callback, scroll_callback, framebuffer_size_callback);

    ShaderMgr::instance().loadShaders();
    ModelMgr::instance().loadModels();
}
