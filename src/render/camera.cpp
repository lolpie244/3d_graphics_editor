#include "render/camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "stage/stage.h"
#include "stage/stage_manager.h"

namespace render {
Camera::Camera(float fov) : fov_(fov) { UpdateProjection(); }

void Camera::Move(float x, float y, float z) {
    Transform::Move(-x, -y, -z);
    origin_ -= glm::vec3{x, y, z};
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

glm::vec3 Camera::GetRealPosition() const {
    auto matrix = glm::translate(glm::mat4(1.0f), origin_);
    matrix = glm::rotate(matrix, glm::radians(-rotation_.z), {0, 0, 1});
    matrix = glm::rotate(matrix, glm::radians(-rotation_.y), {0, 1, 0});
    matrix = glm::rotate(matrix, glm::radians(-rotation_.x), {1, 0, 0});
    matrix = glm::translate(matrix, -origin_);

    return matrix * glm::vec4(GetPosition() * -1.0f, 1.0f);
}

}  // namespace render
