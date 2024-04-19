#include "render/mesh.h"

#include <climits>
#include <iostream>

#include "render/opengl/vertex_layout.h"

namespace render {
VertexLayout Mesh::Vertex::GetLayout() {
    VertexLayout result;

    result.Add<float>(3);  // position
    result.Add<float>(2);  // position
    return result;
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : count_(indices.size()),
      VBO(GL_ARRAY_BUFFER, (void*)vertices.data(), vertices.size() * sizeof(Vertex)),
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

unsigned int Mesh::size() const { return this->count_; }
}  // namespace render
