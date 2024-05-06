#include "math/transform.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace math {
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
void Transform::SetPosition(float x, float y, float z) {
    position_ = glm::vec3(0.0f);
    Move(x, y, z);
}

void Transform::Rotate(float angle, int axis) {
    changed_ = true;
    rotation_ += axis_to_vector(axis) * angle;
}
void Transform::SetRotation(float angle, int axis) {
    rotation_ = glm::vec3(0.0f);
    Rotate(angle, axis);
}

void Transform::SetOrigin(float x, float y, float z) { origin_ = {x, y, z}; }

glm::vec3 Transform::GetPosition() const { return position_; }
glm::vec3 Transform::GetScale() const { return scale_; };
glm::vec3 Transform::GetOrigin() const { return origin_; };
glm::vec3 Transform::GetRotation() const { return rotation_; };

};  // namespace math
