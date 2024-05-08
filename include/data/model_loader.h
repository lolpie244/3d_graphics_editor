#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "render/vertex.h"

namespace data::parser {
template <typename Vertex>
std::pair<std::vector<Vertex>, std::vector<unsigned int>> loadFromFile(const std::string& filename) {
    static_assert(std::is_base_of<render::Vertex<Vertex>, Vertex>::value);

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filename)) {
        if (!reader.Error().empty()) {
            std::cout << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    struct Hash {
		size_t operator()(const Vertex& v) const { return v.Hash(); }
	};

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::unordered_map<Vertex, unsigned int, Hash> unique_vertices;

    auto& attrib = reader.GetAttrib();

    for (const auto& shape : reader.GetShapes()) {
        for (const auto& id : shape.mesh.indices) {
            Vertex vertex;
            vertex.Parse(reader, id);
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
