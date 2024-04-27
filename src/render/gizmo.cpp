#include "render/gizmo.h"

#include <SFML/Graphics/Color.hpp>
#include <algorithm>

#include "math/transform.h"
#include "render/mesh.h"

namespace render {
Gizmo::Gizmo(const std::vector<GizmoVertex>& vertices, const std::vector<unsigned int>& indices)
    : mesh_(vertices, indices) {}

void Gizmo::Draw(data::Shader& shader, Model* model) {
    glClear(GL_DEPTH_BUFFER_BIT);
    shader.setUniform("u_ObjectIndex", Id());
    math::Transform transform = *(math::Transform*)model;

	transform.SetScale(this->GetScale());
    shader.setUniform("u_Color", colors_[0]);
    shader.setUniform("u_Id", math::X);
    mesh_.Draw(GL_TRIANGLES, shader, &transform);

    transform.Rotate(-90, math::Axis::Y);
    shader.setUniform("u_Color", colors_[1]);
    shader.setUniform("u_Id", math::Z);
    mesh_.Draw(GL_TRIANGLES, shader, &transform);

    transform.Rotate(-90, math::Axis::X);
    shader.setUniform("u_Color", colors_[2]);
    shader.setUniform("u_Id", math::Y);
    mesh_.Draw(GL_TRIANGLES, shader, &transform);
}

}  // namespace render
