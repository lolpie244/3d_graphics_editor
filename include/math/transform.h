#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

namespace math {
enum Axis {
    X = 1 << 0,
    Y = 1 << 1,
    Z = 1 << 2,
};

inline glm::vec3 axis_to_vector(unsigned int axis) {
    return {(axis & Axis::X) != 0, (axis & Axis::Y) != 0, (axis & Axis::Z) != 0};
}

struct TransformData {
    glm::vec3 position_ = glm::vec3(0.0f);
    glm::vec3 rotation_ = glm::vec3(0.0f);
    glm::vec3 scale_ = glm::vec3(1.0f);
    glm::vec3 origin_ = glm::vec3(0.0f);
};

class Transform: protected TransformData {
   public:
   public:
    virtual ~Transform() = default;

    virtual void Scale(float x = 1, float y = 1, float z = 1);     // current scale * scale
    virtual void SetScale(float x = 1, float y = 1, float z = 1);  // scale
    virtual void SetScale(glm::vec3 data) { SetScale(data.x, data.y, data.z); }

    virtual void Move(float x = 0, float y = 0, float z = 0);  // current move + move
    virtual void SetPosition(float x = 0, float y = 0, float z = 0);
    virtual void SetPosition(glm::vec3 data) { SetPosition(data.x, data.y, data.z); }

    virtual void Rotate(float angle, int axis);  // current rotate + rotate
    virtual void SetRotation(float angle, int axis);
    virtual void SetRotation(glm::vec3 angles);

    virtual void SetOrigin(float x = 0, float y = 0, float z = 0);

    glm::vec3 GetPosition() const;
    glm::vec3 GetScale() const;
    glm::vec3 GetOrigin() const;
    glm::vec3 GetRotation() const;

	void SetTransformData(const TransformData& other);

   protected:
    mutable bool changed_ = true;
};

class MatrixTransform {
   public:
    virtual const glm::mat4 GetTransformation() const = 0;
};

class LocalModelTransform : virtual public Transform, public MatrixTransform {
   public:
    const glm::mat4 GetTransformation() const;

   protected:
    mutable glm::mat4 final_matrix_;
};

class GlobalModelTransform : virtual public Transform {
   public:
    const glm::mat4 GetTransformation(glm::vec3 origin) const;

   protected:
    mutable glm::mat4 final_matrix_;
};

class ModelTransform : public LocalModelTransform {
   public:
    const glm::mat4 GetTransformation() const override;

    GlobalModelTransform GlobalTransform;
};
}  // namespace math
