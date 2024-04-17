#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "render/mesh.h"
#include "render/vertex_array.h"
#include "utils/shader.h"
#include "utils/singleton.h"

namespace render {
class GL_render : public Singleton<GL_render> {
    friend Singleton<GL_render>;

   public:
	void Clear();
    void Draw(const Mesh& mesh, utils::Shader& shader);
};

}  // namespace render
