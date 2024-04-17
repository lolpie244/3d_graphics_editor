#include "render/render.h"

namespace render {
void GL_render::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void GL_render::Draw(const Mesh& mesh, utils::Shader& shader) {
    sf::Shader::bind(&shader);
	shader.setUniform("u_Transform", mesh.GetTransformation());

    mesh.VAO.Bind();
    glDrawElements(GL_TRIANGLES, mesh.size(), GL_UNSIGNED_INT, NULL);
    mesh.VAO.Unbind();
}
}  // namespace render
