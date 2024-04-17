#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "render/mesh.h"
#include "render/opengl/vertex_array.h"
#include "render/shader.h"
#include "utils/singleton.h"

namespace render {
class GL_render : public Singleton<GL_render> {
    friend Singleton<GL_render>;

   public:
	void Clear();
    void Draw(const Mesh& mesh, render::Shader& shader);
};

}  // namespace render
