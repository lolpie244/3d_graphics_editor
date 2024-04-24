#include "render/gizmo.h"

#include <SFML/Graphics/Color.hpp>

#include "render/mesh.h"

namespace render {
Gizmo::Gizmo(const std::vector<GizmoVertex>& vertices, const std::vector<unsigned int>& indices)
    : mesh_(vertices, indices) {}

void Gizmo::Draw(data::Shader& shader, Model* model) {
	glClear(GL_DEPTH_BUFFER_BIT);
    mesh_.Draw(GL_TRIANGLES, shader, model);
}

}  // namespace render
