#pragma once

#include "math/transform.h"
#include "utils/uuid.h"
namespace render {
class GizmoSupport : virtual public math::ModelTransform, virtual public UUID {};
}  // namespace render
