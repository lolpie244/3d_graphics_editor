#pragma once

#include <glm/ext/matrix_float4x4.hpp>

namespace math {
class Transform {
   public:
    enum Axis {
        X = 1 << 0,
        Y = 1 << 1,
        Z = 1 << 2,
    };

   public:
	virtual ~Transform() = default;

    const glm::mat4& GetTransformation() const;

    virtual void Scale(float x = 1, float y = 1, float z = 1);  // current scale * scale
    void SetScale(float x = 1, float y = 1, float z = 1);       // scale
    void SetScale(glm::vec3 data) { SetScale(data.x, data.y, data.z); }

    virtual void Move(float x = 0, float y = 0, float z = 0);  // current move + move
    void SetPosition(float x = 0, float y = 0, float z = 0);
    void SetPosition(glm::vec3 data) { SetPosition(data.x, data.y, data.z); }

    virtual void Rotate(float angle, Axis axis);  // current rotate + rotate
    void SetRotation(float angle, Axis axis);

    void SetOrigin(float x = 0, float y = 0, float z = 0);

    glm::vec3 Position() const;
    glm::vec3 Scale() const;
    glm::vec3 Origin() const;

   protected:
    glm::vec3 position_ = glm::vec3(0.0f);
    glm::vec3 rotation_ = glm::vec3(0.0f);
    glm::vec3 scale_ = glm::vec3(1.0f);
    glm::vec3 origin_ = glm::vec3(0.0f);

   private:
    mutable glm::mat4 final_matrix_;
    mutable bool changed_ = true;
};
}  // namespace math
