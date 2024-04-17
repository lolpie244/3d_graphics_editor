#include "render/transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace render {
const glm::mat4& Transform::GetTransformation() const {
    if (changed_) {
        changed_ = false;
        final_matrix_ = translation_matrix_ * rotation_matrix_ * scale_matrix_;
    }
    return final_matrix_;
}

void Transform::Scale(float x, float y, float z) {
    changed_ = true;
    scale_matrix_ = glm::scale(scale_matrix_, glm::vec3(x, y, z));
}
void Transform::SetScale(float x, float y, float z) {
    changed_ = true;
    scale_matrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Transform::Move(float x, float y, float z) {
    changed_ = true;
    translation_matrix_ = glm::translate(translation_matrix_, glm::vec3(x, y, z));
}
void Transform::SetMove(float x, float y, float z) {
    changed_ = true;
    translation_matrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Transform::Rotate(float angle, Axis axis) {
    changed_ = true;
    rotation_matrix_ =
        glm::rotate(rotation_matrix_, glm::radians(angle), glm::vec3(axis & Axis::X, axis & Axis::Y, axis & Axis::Z));
}
void Transform::SetRotation(float angle, Axis axis) {
    changed_ = true;
    rotation_matrix_ =
        glm::rotate(rotation_matrix_, glm::radians(angle), glm::vec3(axis & Axis::X, axis & Axis::Y, axis & Axis::Z));
}

};  // namespace render
