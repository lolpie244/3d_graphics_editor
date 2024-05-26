#pragma once

#include "math/transform.h"
namespace render {
class GizmoSupport : virtual public math::ModelTransform {
   public:
    virtual std::pair<glm::vec3, glm::vec3> MeshBox() const = 0;
};
}  // namespace render
