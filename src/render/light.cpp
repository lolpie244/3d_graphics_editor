#include "render/light.h"

#include <SFML/Graphics/Color.hpp>
#include <string>

#include "data/primitives.h"
#include "render/mesh.h"
#include "render/opengl/vertex_layout.h"

namespace render {

VertexLayout LightVertex::Layout() const {
    VertexLayout result;
    result.Add<float>(3);  // position
    return result;
}

Light::Light(LightData data)
    : mesh_(data::primitives::Cube<LightVertex>(0.2), MeshConfig{.changeable = MeshConfig::Static}), Data(data) {}

std::pair<glm::vec3, glm::vec3> Light::MeshBox() const { return mesh_.MeshBox(); }
void Light::Draw(data::Shader& shader) {
    shader.setUniform("u_ObjectId", Id());

    shader.setUniform("u_Color", glm::vec4{Data.color, 1.0f});
    this->mesh_.Draw(GL_TRIANGLES, shader, this);
}

void Light::Apply(int id, data::Shader& shader) {
    std::pair<std::string, glm::vec3> data[]{
        {"position", this->GetPosition()}, {"color", Data.color},       {"ambient", Data.ambient},
        {"diffuse", Data.diffuse},         {"specular", Data.specular},
    };

    std::string current_light = "lights[" + std::to_string(id) + "].";
    for (auto& [name, val] : data) { shader.setUniform(current_light + name, val); }
}

void Light::Scale(float x, float y, float z) {
    Data.ambient += glm::vec3{x};
    Data.diffuse += glm::vec3{y};
    Data.specular += glm::vec3{z};
}

};  // namespace render
