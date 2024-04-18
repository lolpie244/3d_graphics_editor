#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include "transform.h"

namespace render {
class Camera : public Transform {
   public:
	Camera(float fov);
	const glm::mat4 ProjectionMatrix() const;

	void SetFov(float fov);
	void UpdateProjection();

    void Move(float x = 0, float y = 0, float z = 0) override;
	void Rotate(float angle, Axis axis) override;

   private:
    float fov_;
	glm::mat4 projection_matrix_ = glm::mat4(1.0f);
	glm::mat4 view_matrix_ = glm::mat4(1.0f);
};
}  // namespace render
