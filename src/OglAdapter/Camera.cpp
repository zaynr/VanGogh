#include "OglAdapter/Camera.h"
#include "Util/GlobalConfig.h"
#include <cmath>

using namespace VanOGL;
using namespace VanUtil;

Camera::Camera(glm::vec3 pos)
    : pos_(pos),
      front_(glm::vec3(0.0f, 0.0f, -1.0f)),
      up_(glm::vec3(0.0f, 1.0f, 0.0f)),
      worldUp_(glm::vec3(0.0f, 1.0f, 0.0f)),
      right_(glm::vec3(1.0f, 0.0f, 0.0f)),
      theta_(90.0f),
      phi_(270.0f),
      zoom_(45.0f),
      speed_(0.0005f),
      perspective_(true)
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMat()
{
    return glm::lookAt(pos_, pos_ + front_, up_);
}

glm::mat4 Camera::getProjectionMat()
{
    constexpr const float nearPlane = 0.1f;
    constexpr const float farPlane  = 100.0f;

    auto &gi    = GlobalConfig::instance();
    auto width  = static_cast<float>(gi.getScreenWidth());
    auto height = static_cast<float>(gi.getScreenHeight());
    if (perspective_) {
        return glm::perspective(zoom_, width / height, nearPlane, farPlane);
    } else {
        return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
        // return glm::ortho(-width, width, -height, height, nearPlane, farPlane);
    }
}

void Camera::updateCameraTimestamp(const std::chrono::milliseconds &ts)
{
    timestamp_ = ts;
}

void Camera::updateCameraTimestamp()
{
    using namespace std::chrono;
    milliseconds nowtime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    updateCameraTimestamp(nowtime);
}

void Camera::updateCameraPos(const CameraMovement &mv)
{
    using namespace std::chrono;
    auto nowtime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    auto range   = speed_ * (nowtime - timestamp_).count();
    switch (mv) {
        case CameraMovement::UP:
            pos_ += up_ * range;
            break;
        case CameraMovement::DOWN:
            pos_ -= up_ * range;
            break;
        case CameraMovement::FORWARD:
            pos_ += front_ * range;
            break;
        case CameraMovement::BACKWARD:
            pos_ -= front_ * range;
            break;
        case CameraMovement::RIGHT:
            pos_ += right_ * range;
            break;
        case CameraMovement::LEFT:
            pos_ -= right_ * range;
            break;

        default:
            break;
    }
}

void Camera::updateCameraFace(const int32_t xoffset, const int32_t yoffset)
{
    constexpr const float sensi = 0.05f;

    auto theta = theta_ - yoffset * sensi;
    auto phi   = phi_ + xoffset * sensi;
    if (theta < 0.001f) {
        theta_ = 0.001f;
    } else if (theta > 179.99f) {
        theta_ = 179.99f;
    } else {
        theta_ = theta;
    }
    if (phi > 360.0f) {
        phi -= 360.0f;
    }
    phi_ = phi;

    updateCameraProperty();
}

void Camera::updateCameraProperty()
{
    front_.x = sin(glm::radians(theta_)) * cos(glm::radians(phi_));
    front_.y = cos(glm::radians(theta_));
    front_.z = sin(glm::radians(theta_)) * sin(glm::radians(phi_));

    right_ = glm::cross(front_, worldUp_);
    up_    = glm::cross(right_, front_);

    glm::normalize(up_);
    glm::normalize(front_);
    glm::normalize(right_);
}

void Camera::speedUp()
{
    speed_ += 0.0005f;
}

void Camera::slowDown()
{
    if (speed_ < 0.001f) {
        return;
    }
    speed_ -= 0.0005f;
}

void Camera::switchProjection()
{
    perspective_ = !perspective_;
}
