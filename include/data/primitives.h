#pragma once

#include <glm/ext/vector_float3.hpp>
#include <vector>

#include "render/mesh.h"
namespace data::primitives {

template <typename Vertex>
inline render::Mesh<Vertex>::RawMesh Cube(float size) {
    size /= 2.0f;

    std::vector<Vertex> vertices = {
        {.position = {-size, -size, -size}}, {.position = {size, -size, -size}}, {.position = {size, size, -size}},
        {.position = {-size, size, -size}},  {.position = {-size, -size, size}}, {.position = {size, -size, size}},
        {.position = {size, size, size}},    {.position = {-size, size, size}},
    };

    std::vector<typename render::Mesh<Vertex>::Face> indices = {
        {.indices = {0, 1, 2, 3}}, {.indices = {4, 5, 6, 7}}, {.indices = {0, 1, 5, 4}},
        {.indices = {2, 3, 7, 6}}, {.indices = {0, 3, 7, 4}}, {.indices = {1, 2, 6, 5}},
    };

    return {vertices, indices};
}
}  // namespace data::primitive
