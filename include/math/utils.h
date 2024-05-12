#pragma once

#include <glm/glm.hpp>

namespace math {
inline bool point_in_triangle(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 P) {
    float s1 = C.y - A.y;
    float s2 = C.x - A.x;
    float s3 = B.y - A.y;
    float s4 = P.y - A.y;

    float w1 = (A.x * s1 + s4 * s2 - P.x * s1) / (s3 * s2 - (B.x - A.x) * s1);
    float w2 = (s4 - w1 * s3) / s1;
    return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;
}

template <typename T>
inline float angle(T A, T B, T origin) {
    glm::vec2 da = glm::normalize(A - origin);
    glm::vec2 db = glm::normalize(B - origin);
    return glm::degrees(glm::acos(glm::dot(da, db)));
}
}  // namespace math
