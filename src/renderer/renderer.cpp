#include "renderer/renderer.h"

namespace renderer {
void GL_Renderer::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void GL_Renderer::Draw(const Mesh& mesh, const sf::Shader& shader) {
    sf::Shader::bind(&shader);
    mesh.VAO.Bind();
    glDrawElements(GL_TRIANGLES, mesh.size(), GL_UNSIGNED_INT, NULL);
    mesh.VAO.Unbind();
}
}  // namespace renderer
