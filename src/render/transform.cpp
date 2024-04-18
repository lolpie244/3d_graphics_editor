#include "render/transform.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace render {
const glm::mat4& Transform::GetTransformation() const {
    if (changed_) {
        changed_ = false;
        final_matrix_ = glm::translate(glm::mat4(1.0f), position_ + origin_);
        final_matrix_ = glm::rotate(final_matrix_, glm::radians(rotation_.x), {1, 0, 0});
        final_matrix_ = glm::rotate(final_matrix_, glm::radians(rotation_.y), {0, 1, 0});
        final_matrix_ = glm::rotate(final_matrix_, glm::radians(rotation_.z), {0, 0, 1});
        final_matrix_ = glm::scale(final_matrix_, scale_);
        final_matrix_ = glm::translate(final_matrix_, -origin_);
    }
    return final_matrix_;
}

void Transform::Scale(float x, float y, float z) {
    changed_ = true;
    scale_ *= glm::vec3{x, y, z};
}
void Transform::SetScale(float x, float y, float z) {
    scale_ = glm::vec3(1.0f);
    Scale(x, y, z);
}

void Transform::Move(float x, float y, float z) {
    changed_ = true;
    position_ += glm::vec3{x, y, z};
}
void Transform::SetMove(float x, float y, float z) {
    position_ = glm::vec3(0.0f);
    Move(x, y, z);
}

void Transform::Rotate(float angle, Axis axis) {
    changed_ = true;
    rotation_ += glm::vec3{angle * (axis & Axis::X), angle * (axis & Axis::Y), angle * (axis & Axis::Z)};
}
void Transform::SetRotation(float angle, Axis axis) {
    rotation_ = glm::vec3(0.0f);
    Rotate(angle, axis);
}

void Transform::SetOrigin(float x, float y, float z) { origin_ = {x, y, z}; }

};  // namespace render