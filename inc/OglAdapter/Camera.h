#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <vector>

namespace VanOGL {

enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class Camera {
public:
    Camera(glm::vec3 pos);

    ~Camera();

    glm::mat4 getViewMat();

    glm::mat4 getProjectionMat();

    void updateCameraTimestamp(const std::chrono::milliseconds &ts);

    void updateCameraTimestamp();

    void updateCameraPos(const CameraMovement &mv);

    void updateCameraFace(const int32_t xoffset, const int32_t yoffset);

    void speedUp();

    void slowDown();

private:
    void updateCameraProperty();

private:
    // basic property
    glm::vec3 pos_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 worldUp_;
    glm::vec3 right_;

    // Spherical angles
    float theta_;
    float phi_;

    // configure
    float zoom_;
    float speed_;
    std::chrono::milliseconds timestamp_;
    bool perspective_;
};

}; // namespace VanOGL
