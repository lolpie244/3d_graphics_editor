#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "renderer/mesh.h"
#include "renderer/vertex_array.h"
#include "utils/singleton.h"

namespace renderer {
class GL_Renderer : public Singleton<GL_Renderer> {
    friend Singleton<GL_Renderer>;

   public:
	void Clear();
    void Draw(const Mesh& mesh, const sf::Shader& shader);
};

}  // namespace renderer
