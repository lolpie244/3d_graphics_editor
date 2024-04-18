#pragma once

#include <glm/ext/matrix_float4x4.hpp>

namespace render {
class Transform {
   public:
    enum Axis {
        X = 1 << 0,
        Y = 1 << 1,
        Z = 1 << 2,
    };

   public:
    const glm::mat4& GetTransformation() const;

    virtual void Scale(float x = 1, float y = 1, float z = 1);     // current scale * scale
    void SetScale(float x = 1, float y = 1, float z = 1);  // scale

    virtual void Move(float x = 0, float y = 0, float z = 0);  // current move + move
    void SetMove(float x = 0, float y = 0, float z = 0);

    virtual void Rotate(float angle, Axis axis);  // current rotate + rotate
    void SetRotation(float angle, Axis axis);

	void SetOrigin(float x = 0, float y = 0, float z = 0);

   private:
    glm::vec3 position_ = glm::vec3(0.0f);
    glm::vec3 rotation_ = glm::vec3(0.0f);
    glm::vec3 scale_ = glm::vec3(1.0f);
	glm::vec3 origin_ = glm::vec3(0.0f);

    mutable glm::mat4 final_matrix_;
    mutable bool changed_ = true;
};
}  // namespace render
