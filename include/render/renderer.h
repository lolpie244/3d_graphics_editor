#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "render/camera.h"
#include "render/mesh.h"
#include "render/opengl/vertex_array.h"
#include "data/shader.h"
#include "utils/settings.h"
#include "utils/singleton.h"

namespace render {
class GL_render : public Singleton<GL_render> {
    friend Singleton<GL_render>;

   public:
    void Clear();
    void Draw(const Mesh& mesh, data::Shader& shader);

   protected:
    GL_render() = default;
};

}  // namespace render
