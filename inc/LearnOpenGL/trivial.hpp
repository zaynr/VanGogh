#pragma once

#include "OglAdapter/GlfwMgr.h"
#include "OglAdapter/Camera.h"
#include "OglAdapter/TextureMgr.h"
#include "OglAdapter/Shader.h"
#include "OglAdapter/ModelMgr.h"
#include "OglAdapter/ShaderMgr.h"
#include "OglAdapter/FrameBuffer.h"
#include "Util/FileSystem.h"
#include "Util/GlobalConfig.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace VanOGL;
using namespace VanUtil;

// global var
unsigned int uboMatrices;

// meshes
unsigned int planeVAO;
// quad
unsigned int quadVAO;
unsigned int quadVBO;

// camera
Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));

void prepareGlobal()
{
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // second param: binding point, fourth param: offset
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
}

void updateGlobalCamera()
{
    auto cameraProj = mainCamera.getProjectionMat();
    auto cameraView = mainCamera.getViewMat();

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(cameraProj));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cameraView));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

float planeVertices[] = {
    // positions            // normals         // texcoords
     25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
    -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
    -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

     25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
    -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
     25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
};

void prepareFloor()
{
    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
}

void drawFloor(Shader &shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureMgr::instance().getTexture("assets/texture/wood.png"));
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

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
    static bool flip             = true;
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
        mainCamera.switchHold();
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
    TextureMgr::instance().loadTextures();

    prepareGlobal();
    prepareFloor();
}
