#include "renderer/mesh.h"

#include "renderer/vertex_layout.h"

namespace renderer {
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
    VAO.AddBuffer(VBO, Vertex::GetLayout());
    VAO.AddBuffer(IBO);
}

unsigned int Mesh::size() const {
	return this->count_;
}
}  // namespace renderer
