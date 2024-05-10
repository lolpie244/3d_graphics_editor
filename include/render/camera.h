#pragma once

#include <glm/ext/matrix_float4x4.hpp>

#include "math/transform.h"

namespace render {
class Camera : public math::Transform {
   public:
    Camera(float fov);
    const glm::mat4 ProjectionMatrix() const;

    void SetFov(float fov);
    void UpdateProjection();

    void Move(float x = 0, float y = 0, float z = 0) override;
    void Rotate(float angle, int axis) override;

    glm::vec3 GetRealPosition() const;
   private:
    float fov_;
    glm::mat4 projection_matrix_ = glm::mat4(1.0f);
};
}  // namespace render
