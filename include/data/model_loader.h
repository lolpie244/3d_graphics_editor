#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "render/gizmo.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "render/model.h"

namespace data::parser {

template <typename T>
struct Vec3Hash {
    size_t operator()(const render::ModelVertex& v) const {
        static_assert(false);
        return 0;
    }
};

template <typename T>
T Parse(const tinyobj::ObjReader& attr, tinyobj::index_t id) {
    static_assert(false);
}
template <typename Vertex>
static std::pair<std::vector<Vertex>, std::vector<unsigned int>> loadFromFile(const std::string& filename);
}  // namespace data::parser

#include "gizmo.h"
#include "model.h"

namespace data::parser {
template <typename Vertex>
static std::pair<std::vector<Vertex>, std::vector<unsigned int>> loadFromFile(const std::string& filename) {
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filename)) {
        if (!reader.Error().empty()) {
            std::cout << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::unordered_map<Vertex, unsigned int, Vec3Hash<Vertex>> unique_vertices;

    auto& attrib = reader.GetAttrib();

    for (const auto& shape : reader.GetShapes()) {
        for (const auto& id : shape.mesh.indices) {
            Vertex vertex = Parse<Vertex>(reader, id);
            if (unique_vertices.count(vertex) == 0) {
                unique_vertices[vertex] = vertices.size();
                vertices.push_back(vertex);
            }
            indices.push_back(unique_vertices[vertex]);
        }
    }
    return {vertices, indices};
}
}  // namespace data::parser
