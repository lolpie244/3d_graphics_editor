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

    void Scale(float x = 1, float y = 1, float z = 1);     // current scale * scale
    void SetScale(float x = 1, float y = 1, float z = 1);  // scale

    void Move(float x = 0, float y = 0, float z = 0);  // current move + move
    void SetMove(float x = 0, float y = 0, float z = 0);

    void Rotate(float angle, Axis axis);  // current rotate + rotate
    void SetRotation(float angle, Axis axis);

   private:
    glm::mat4 translation_matrix_ = glm::mat4(1.0f);
    glm::mat4 rotation_matrix_ = glm::mat4(1.0f);
    glm::mat4 scale_matrix_ = glm::mat4(1.0f);

    mutable glm::mat4 final_matrix_;
    mutable bool changed_ = true;
};
}  // namespace render
