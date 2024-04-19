#include "render/renderer.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Glsl.hpp>

#include "stage/stage.h"
#include "stage/stage_manager.h"

namespace render {
void GL_render::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void GL_render::Draw(const Mesh& mesh, data::Shader& shader, unsigned int mode) {
    sf::Shader::bind(&shader);
    auto& camera = stage::StageManager::Instance().Camera();

    shader.setUniform("u_Projection", camera->ProjectionMatrix());
    shader.setUniform("u_View", camera->GetTransformation());
    shader.setUniform("u_Model", mesh.GetTransformation());
	shader.setUniform("u_Texture", mesh.texture);

    mesh.VAO.Bind();
    glDrawElements(mode, mesh.size(), GL_UNSIGNED_INT, NULL);
    mesh.VAO.Unbind();
}
}  // namespace render
