#pragma once

#include <climits>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <list>
#include <vector>

#include "utils.h"
#include "utils/singleton.h"

namespace math {

class EarCuttingTriangulate : public Singleton<EarCuttingTriangulate> {
    friend Singleton<EarCuttingTriangulate>;

   public:
    std::vector<unsigned int> Traingulate(const std::vector<glm::vec3> positions) {
        if (positions.size() == 3)
            return {0, 1, 2};

        auto vertexes = project_vertexes(positions);
		bool is_clockwise = is_clockwise_polygon(vertexes);
        std::vector<unsigned int> result;

        auto it = vertexes.begin();
        int i = 0;
        while (vertexes.size() > 3) {
            auto prev = it != vertexes.begin() ? std::prev(it) : std::prev(vertexes.end());
            auto next = it != std::prev(vertexes.end()) ? std::next(it) : vertexes.begin();

            glm::vec2 a = prev->second - it->second;
            glm::vec2 b = next->second - it->second;

            double angle = a.x * b.y - a.y * b.x;

            if (angle > 0 && !is_clockwise || angle < 0 && is_clockwise) {
                it = next;
                continue;
            }

            bool point_inside = false;
            for (auto point : vertexes) {
                if (point != *prev && point != *it && point != *next &&
                    point_in_triangle(prev->second, it->second, next->second, point.second)) {
                    point_inside = true;
                    continue;
                }
            }
            if (point_inside) {
                it = next;
                continue;
            }
            result.push_back(prev->first);
            result.push_back(it->first);
            result.push_back(next->first);

            vertexes.erase(it);
            it = next;
        }

		for (auto& [i, _] : vertexes)
			result.push_back(i);

        return result;
    }

   private:
    std::list<std::pair<int, glm::vec2>> project_vertexes(const std::vector<glm::vec3> positions) {
        glm::vec3 min = {INT_MAX, INT_MAX, INT_MAX}, max{-INT_MAX, -INT_MAX, -INT_MAX};
        for (auto position : positions) {
            min = {std::min(min.x, position.x), std::min(min.y, position.y), std::min(min.z, position.z)};
            max = {std::max(max.x, position.x), std::max(max.y, position.y), std::max(max.z, position.z)};
        }
        int min_dimension = 0;
        for (int i = 0; i < 3; i++)
            if (max[i] - min[i] < max[min_dimension] - min[min_dimension])
                min_dimension = i;

        std::list<std::pair<int, glm::vec2>> vertexes;
        for (int i = 0; i < positions.size(); i++) {
            glm::vec3 vertex = positions[i];
            std::swap(vertex[min_dimension], vertex[2]);
            vertexes.push_back({i, vertex});
        }

        return vertexes;
    }

    bool is_clockwise_polygon(const std::list<std::pair<int, glm::vec2>>& polygon) {
		// if sum [(x2 - x1)(y2 + y1)] >= 0 -> clockwise
        float sum = 0;
        auto it = polygon.begin();
        for (; it != std::prev(polygon.end()); it++) {
            auto next = std::next(it);
            sum += (next->second.x - it->second.x) * (next->second.y + it->second.y);
        }
        sum += (polygon.begin()->second.x - it->second.x) * (polygon.begin()->second.y + it->second.y);

        return sum > 0;
    }
};

}  // namespace math
