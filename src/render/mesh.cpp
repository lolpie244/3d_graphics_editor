#include "render/mesh.h"

#include <climits>
#include <glm/ext/vector_float3.hpp>
#include <iostream>

#include "render/opengl/vertex_layout.h"

namespace render {
VertexLayout Mesh::Vertex::GetLayout() {
    VertexLayout result;

    result.Add<float>(3);  // position
    result.Add<float>(2);  // texture coords
    return result;
}

std::atomic<int> Mesh::max_object_id{1};

int Mesh::Id() const { return id_; }
unsigned int Mesh::size() const { return this->count_; }

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Change is_changeable)
    : vertices_(vertices),
      indices_(indices),
      count_(indices.size()),
      changeable_(is_changeable),
      VBO(GL_ARRAY_BUFFER, (void*)vertices.data(), vertices.size() * sizeof(Vertex), is_changeable),
      IBO(GL_ELEMENT_ARRAY_BUFFER, (void*)indices.data(), indices.size() * sizeof(Vertex)) {
    glm::vec3 min = {INT_MAX, INT_MAX, INT_MAX}, max{-INT_MAX, -INT_MAX, -INT_MAX};

    for (auto& vertex : vertices) {
        min = {std::min(min.x, vertex.position.x), std::min(min.y, vertex.position.y),
               std::min(min.z, vertex.position.z)};

        max = {std::max(max.x, vertex.position.x), std::max(max.y, vertex.position.y),
               std::max(max.z, vertex.position.z)};
    }
    VAO.AddBuffer(VBO, Vertex::GetLayout());
    VAO.AddBuffer(IBO);
    this->SetOrigin((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
}

const Mesh::Vertex Mesh::GetVertex(int id) const { return vertices_[id]; }

const Mesh::Vertex Mesh::GetVertexByIndex(int id) const { return vertices_[indices_[id]]; }

void Mesh::SetVertex(int id, Vertex data) {
    if (changeable_ == Change::Disable) {
        std::cout << "Try to change unchangeable mesh";
        exit(1);
    }

    VBO.Write(id * sizeof(Vertex), &data, sizeof(data));
    vertices_[id] = data;
}

void Mesh::SetVertexPosition(int id, glm::vec3 new_position) {
    auto old_position = vertices_[id].position;
    for (int i = 0; i < vertices_.size(); i++) {
        if (vertices_[i].position == old_position) {
            auto new_data = vertices_[i];
            new_data.position = new_position;
            SetVertex(i, new_data);
        }
    }
}

}  // namespace render
