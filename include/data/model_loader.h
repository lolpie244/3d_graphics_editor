#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "render/mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "render/vertex.h"

namespace data::parser {

template <typename Vertex>
render::Mesh<Vertex>::RawMesh loadModelFromFile(const std::string& filename) {
    static_assert(std::is_base_of<render::Vertex<Vertex>, Vertex>::value);

    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig config;
    config.triangulate = false;

    if (!reader.ParseFromFile(filename, config)) {
        if (!reader.Error().empty()) {
            std::cout << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    struct Hash {
        size_t operator()(const Vertex& v) const { return v.Hash(); }
    };

    std::vector<Vertex> vertices;
    std::vector<typename render::Mesh<Vertex>::Face> faces(1);
    std::unordered_map<Vertex, unsigned int, Hash> unique_vertices;

    for (const auto& shape : reader.GetShapes()) {
        int i = -1;
        auto current_face = shape.mesh.num_face_vertices.begin();
        for (const auto& id : shape.mesh.indices) {
            i++;
            if (*current_face <= i) {
                faces.emplace_back();
                current_face++;
                i = 0;
            }

            Vertex vertex;
            vertex.Parse(reader, id);
            if (unique_vertices.count(vertex) == 0) {
                unique_vertices[vertex] = vertices.size();
                vertices.push_back(vertex);
            }
            faces.back().indices.push_back(unique_vertices[vertex]);
        }
        return typename render::Mesh<Vertex>::RawMesh{
            .vertices = vertices,
            .faces = faces,
        };
    }
    return {};
}
}  // namespace data::parser
