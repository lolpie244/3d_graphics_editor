#include "render/renderer.h"
#include <iostream>

namespace render {
void GL_render::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void GL_render::Draw(const Mesh& mesh, render::Shader& shader) {
    sf::Shader::bind(&shader);
	shader.setUniform("u_Projection", Camera.ProjectionMatrix());
	shader.setUniform("u_View", Camera.GetTransformation());
	shader.setUniform("u_Model", mesh.GetTransformation());

    mesh.VAO.Bind();
    glDrawElements(GL_TRIANGLES, mesh.size(), GL_UNSIGNED_INT, NULL);
    mesh.VAO.Unbind();
}
}  // namespace render
