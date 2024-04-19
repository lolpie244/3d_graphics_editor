#pragma once

#include <glm/ext/vector_float3.hpp>

#include "math/vector2.h"
#include "stage/stage_manager.h"

namespace math {
inline glm::vec3 to_ndc(glm::vec3 point) {
    auto window_size = stage::StageManager::Instance().windowSize();

    return {
        2 * point.x / window_size.x - 1.0f,
        2 * point.y / window_size.y - 1.0f,
        point.z,
    };
}

inline glm::vec3 to_ndc(math::Vector2f point) { return to_ndc({point.x, point.y, 0}); }

inline glm::vec3 to_pixel(glm::vec3 point) {
    auto window_size = stage::StageManager::Instance().windowSize();

    return {
        (point.x + 1.0f * window_size.x) / 2.0f,
        (point.y + 1.0f * window_size.y) / 2.0f,
        point.z,
    };
}

}  // namespace math
