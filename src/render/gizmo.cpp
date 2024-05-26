#include "render/gizmo.h"

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>
#include <glm/gtx/vector_angle.hpp>

#include "data/model_loader.h"
#include "math/ray.h"
#include "math/transform.h"
#include "render/mesh.h"

namespace render {
VertexLayout GizmoVertex::Layout() const {
    VertexLayout result;
    result.Add<float>(3);  // position
    return result;
}

size_t GizmoVertex::Hash() const {
    auto fhash = std::hash<float>{};
    return fhash(position.x) ^ (fhash(position.y) << 1) ^ (fhash(position.z) << 2);
}

void GizmoVertex::Parse(const tinyobj::ObjReader& reader, tinyobj::index_t id) {
    auto& attrib = reader.GetAttrib();

    position = {
        attrib.vertices[3 * size_t(id.vertex_index) + 0],
        attrib.vertices[3 * size_t(id.vertex_index) + 1],
        attrib.vertices[3 * size_t(id.vertex_index) + 2],
    };
}

Gizmo::Gizmo(const Mesh<GizmoVertex>::RawMesh& mesh)
    : mesh_(mesh, MeshConfig{.changeable = MeshConfig::Static, .triangulate = true}) {}

void Gizmo::Draw(data::Shader& shader) {
    if (current_model_ == nullptr)
        return;

    glClear(GL_DEPTH_BUFFER_BIT);
    shader.setUniform("u_ObjectId", Id());
    this->SetRotation({0, 0, 0});
    this->SetPosition(0);

    math::ModelTransform transform = *(math::ModelTransform*)current_model_;

    transform.SetScale(1, 1, 1);
    shader.setUniform("u_Color", sf::Color::Green);
    shader.setUniform("u_Data", math::X);
    mesh_.Draw(GL_TRIANGLES, shader, transform.GetTransformation() * this->GetTransformation());

    this->Rotate(-90, math::Y);
    shader.setUniform("u_Color", sf::Color::Red);
    shader.setUniform("u_Data", math::Z);
    mesh_.Draw(GL_TRIANGLES, shader, transform.GetTransformation() * this->GetTransformation());

    this->Rotate(90, math::Z);
    shader.setUniform("u_Color", sf::Color::Blue);
    shader.setUniform("u_Data", math::Y);
    mesh_.Draw(GL_TRIANGLES, shader, transform.GetTransformation() * this->GetTransformation());
}

void Gizmo::SetModel(render::Model* model) {
    current_model_ = model;

    if (model == nullptr)
        return;

    auto [min, max] = model->ModelMesh().MeshBox();
    min = model->GetTransformation() * glm::vec4(min, 1.0f);
    max = model->GetTransformation() * glm::vec4(max, 1.0f);

    float new_scale = INT_MAX;
    auto gizmo_box = this->mesh_.MeshBox();

    for (int i = 0; i < 3; i++)
        new_scale = std::min(new_scale, (max[i] - min[i]) / (gizmo_box.second[i] - gizmo_box.first[i]));

    this->SetScale(new_scale, new_scale, new_scale);
}

void Gizmo::Reset() { current_model_ = nullptr; }

glm::vec3 Gizmo::Normal(unsigned int axis) const {
    return current_model_->GetTransformation() * glm::vec4(math::axis_to_vector(axis), 1.0f);
}

void TranslateGizmo::MouseMove(glm::vec2 mouse_position, glm::vec3 mouse_moved, unsigned int axis) {
    mouse_moved = mouse_moved * Normal(axis) * math::axis_to_vector(axis);
    current_model_->Move(mouse_moved.x, mouse_moved.y, mouse_moved.z);
}

void ScaleGizmo::MouseMove(glm::vec2 mouse_position, glm::vec3 mouse_moved, unsigned int axis) {
    mouse_moved = mouse_moved * Normal(axis) * math::axis_to_vector(axis);
    auto scale = current_model_->GetScale() + mouse_moved;
    current_model_->SetScale(scale.x, scale.y, scale.z);
}

void RotateGizmo::MousePress() { last_position = {-1, -1, -1}; }

void RotateGizmo::MouseMove(glm::vec2 mouse_position, glm::vec3 mouse_moved, unsigned int axis) {
    auto ray = math::Ray::FromPoint(mouse_position);
    auto gizmo_center = current_model_->GetPosition();

    glm::vec3 normal = Normal(axis);
    normal = glm::normalize(normal);
    auto new_point = ray.PlainIntersection(gizmo_center, normal);

    if (last_position == glm::vec3{-1, -1, -1}) {
        last_position = new_point;
        return;
    }

    auto old_vec = (last_position - gizmo_center);
    auto new_vec = (new_point - gizmo_center);

    float angle = glm::degrees(glm::orientedAngle(glm::normalize(old_vec), glm::normalize(new_vec), normal));

    current_model_->GlobalTransform.Rotate(-1 * angle, axis);
    last_position = new_point;
}

}  // namespace render
