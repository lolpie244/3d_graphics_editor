#pragma once

#include <bits/iterator_concepts.h>

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "mesh.h"
#include "render/mesh.h"

namespace data {
class Mesh {
   public:
    static std::shared_ptr<render::Mesh> loadFromFile(const std::string& filename) {
        tinyobj::ObjReader reader;
        if (!reader.ParseFromFile(filename)) {
            if (!reader.Error().empty()) {
                std::cout << "TinyObjReader: " << reader.Error();
            }
            exit(1);
        }

        std::vector<render::Mesh::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::unordered_map<glm::vec3, unsigned int, Vec3Hash> unique_vertices;

        auto& attrib = reader.GetAttrib();

        for (const auto& shape : reader.GetShapes()) {
            for (const auto& id : shape.mesh.indices) {
                render::Mesh::Vertex vertex;
                vertex.position = {attrib.vertices[3 * size_t(id.vertex_index) + 0],
                                   attrib.vertices[3 * size_t(id.vertex_index) + 1],
                                   attrib.vertices[3 * size_t(id.vertex_index) + 2]};

                if (id.texcoord_index >= 0) {
                    vertex.texture_coords = {attrib.texcoords[2 * size_t(id.texcoord_index) + 0],
                                             attrib.texcoords[2 * size_t(id.texcoord_index) + 1]};
                }

                if (unique_vertices.count(vertex.position) == 0) {
                    unique_vertices[vertex.position] = vertices.size();
                    vertices.push_back(vertex);
                }

                indices.push_back(unique_vertices[vertex.position]);
            }
        }
        return std::make_shared<render::Mesh>(vertices, indices);
    }

   private:
    struct Vec3Hash {
        size_t operator()(const glm::vec3& v) const {
            return std::hash<float>{}(v.x) ^ (std::hash<float>{}(v.y) << 1) ^ (std::hash<float>{}(v.z) << 2);
        }
    };
};
}  // namespace data

