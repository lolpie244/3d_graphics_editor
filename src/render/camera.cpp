#include "render/camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "stage/stage.h"
#include "stage/stage_manager.h"

namespace render {
Camera::Camera(float fov) : fov_(fov) { UpdateProjection(); }

void Camera::Move(float x, float y, float z) {
    Transform::Move(-x, -y, -z);
    origin_.x = this->position_.x;
    origin_.y = this->position_.y;
}
void Camera::Rotate(float angle, int axis) { Transform::Rotate(-angle, axis); }

void Camera::SetFov(float fov) {
    fov_ = fov;
    UpdateProjection();
}

void Camera::UpdateProjection() {
    auto window = stage::StageManager::Instance().windowSize();
    projection_matrix_ = glm::perspective(glm::radians(fov_), (float)window.x / window.y, 0.1f, 100.0f);
}
const glm::mat4 Camera::ProjectionMatrix() const { return projection_matrix_; }

}  // namespace render
