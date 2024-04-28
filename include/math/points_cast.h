#pragma once

#include <glm/ext/vector_float3.hpp>


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

inline glm::vec3 to_ndc(glm::vec2 point) { return to_ndc({point.x, point.y, 0}); }

inline glm::vec3 to_pixel(glm::vec3 point) {
    auto window_size = stage::StageManager::Instance().windowSize();

    return {
        (point.x / window_size.x) * 2.0f - 1,
        (point.y / window_size.y) * 2.0f - 1,
        point.z,
    };
}

inline glm::vec3 to_world_coords(glm::vec2 point) {
    auto& camera = stage::StageManager::Instance().Camera();

    glm::vec4 ndc_coords = glm::vec4(to_ndc({point.x, point.y, 0}), 1.0f);
	ndc_coords.y *= -1;
    glm::vec4 eye_space = glm::inverse(camera->ProjectionMatrix()) * ndc_coords;
	return glm::inverse(camera->GetTransformation()) * glm::vec4(eye_space.x, eye_space.y, 0, 0);
}

}  // namespace math
