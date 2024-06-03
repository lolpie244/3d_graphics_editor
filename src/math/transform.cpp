#include "math/transform.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace math {

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
    rotation_ = rotation_ * axis_to_vector(Axis::X | Axis::Y | Axis::Z - axis);
    Rotate(angle, axis);
}

void Transform::SetRotation(glm::vec3 angles) {
    rotation_ = angles;
    changed_ = true;
}

void Transform::SetOrigin(float x, float y, float z) { origin_ = {x, y, z}; }

glm::vec3 Transform::GetPosition() const { return position_; }
glm::vec3 Transform::GetScale() const { return scale_; };
glm::vec3 Transform::GetOrigin() const { return origin_; };
glm::vec3 Transform::GetRotation() const { return rotation_; };

TransformData Transform::GetTransformData() const { return *this; }

void Transform::SetTransformData(const TransformData& other) {
    changed_ = true;

    this->position_ = other.position_;
    this->rotation_ = other.rotation_;
    this->scale_ = other.scale_;
    this->origin_ = other.origin_;
}

};  // namespace math
