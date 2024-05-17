#pragma once

#define GLM_SWIZZLE_XYZW
#define GLM_SWIZZLE_STQP
#include <glm/glm.hpp>

#include "points_cast.h"
#include "stage/stage_manager.h"

namespace math {
struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    static Ray FromPoint(glm::vec2 point) {
        auto& camera = stage::StageManager::Instance().Camera();

        glm::vec4 ndc_coords = glm::vec4(to_ndc({point.x, point.y, -1.0f}), 1.0f);
        glm::vec4 eye_space = glm::inverse(camera->ProjectionMatrix()) * ndc_coords;
        glm::vec3 direction = glm::inverse(camera->GetTransformation()) * glm::vec4(eye_space.x, eye_space.y, -1.0, 0);

        return {camera->GetRealPosition(), glm::normalize(direction)};
    }

    glm::vec3 PlainIntersection(glm::vec3 plane_point, glm::vec3 normal) {
        float t = glm::dot(plane_point - origin, normal) / glm::dot(direction, normal);
        return origin + direction * t;
    }

    glm::vec3 SphereIntersection(glm::vec3 sphere_position) {
        float t = glm::dot(sphere_position - origin, direction);
        return origin + direction * t;
    }
};
}  // namespace math
