#pragma once

#include "math/transform.h"
#include "utils/uuid.h"
namespace render {
class GizmoSupport : virtual public math::ModelTransform, virtual public UUID {
   public:
    virtual std::pair<glm::vec3, glm::vec3> MeshBox() const = 0;
};
}  // namespace render
