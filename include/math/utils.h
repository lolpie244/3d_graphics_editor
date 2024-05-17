#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

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

template <int N>
glm::vec<N - 1, float> erase(glm::vec<N, float> vec, int id) {
    glm::vec<N - 1, float> result;
    for (int i = 0; i < vec.length(); i++) {
        if (i == id)
            continue;
        result[i - (id < i)] = vec[i];
    }

    return result;
}

template <typename Vertex>
inline void sort_clockwise_polygon(const std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    glm::vec3 centroid(0.0f);
    for (auto& id : indices) { centroid += vertices[id].position; }
    centroid /= (float)(indices.size());

    std::sort(indices.begin(), indices.end(), [&vertices, centroid](unsigned int a_id, unsigned int b_id) {
		glm::vec3 a = vertices[a_id].position;
		glm::vec3 b = vertices[b_id].position;

		glm::vec2 diffa(a.x - centroid.x, a.y - centroid.y);
		glm::vec2 diffb(b.x - centroid.x, b.y - centroid.y);

		return std::atan2(diffa.y, diffa.x) < std::atan2(diffb.y, diffb.x);
	});
}
}  // namespace math
