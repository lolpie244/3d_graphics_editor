#include "render/gizmo.h"

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>

#include "data/model_loader.h"
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

Gizmo::Gizmo(const std::vector<GizmoVertex>& vertices, const std::vector<unsigned int>& indices)
    : mesh_(vertices, indices) {}

std::unique_ptr<Gizmo> Gizmo::loadFromFile(const std::string& filename) {
    auto data = data::parser::loadModelFromFile<GizmoVertex>(filename);
    return std::make_unique<Gizmo>(data.first, data.second);
}

void Gizmo::Draw(data::Shader& shader, Model* model) {
    glClear(GL_DEPTH_BUFFER_BIT);
    shader.setUniform("u_ObjectId", Id());
	this->SetRotation({0, 0, 0});
	this->SetPosition(0);

    math::Transform transform = *(math::Transform*)model;

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

glm::vec3 Gizmo::VertexPosition(int id) { return mesh_.Vertices()[id].position; }

}  // namespace render
