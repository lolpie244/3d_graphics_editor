#pragma once

#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

#include "math/transform.h"
#include "render/opengl/vertex_array.h"
#include "render/opengl/vertex_buffer.h"
#include "render/opengl/vertex_layout.h"

namespace render {
class Mesh : public math::Transform {
   public:
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texture_coords;

        bool operator==(const Vertex& b) const { return position == b.position && texture_coords == b.texture_coords; }

        static VertexLayout GetLayout();
    };

   public:
    unsigned int size() const;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

   public:
    VertexArray VAO;
    VertexBuffer VBO;
    VertexBuffer IBO;

    sf::Texture texture;

   private:
    size_t count_;
};
}  // namespace render
