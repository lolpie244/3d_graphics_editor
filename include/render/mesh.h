#pragma once

#include <SFML/Graphics/Glsl.hpp>
#include <vector>

#include "render/transform.h"
#include "render/vertex_array.h"
#include "render/vertex_buffer.h"
#include "render/vertex_layout.h"

namespace render {
class Mesh : public Transform {
	public:

    struct Vertex {
		sf::Glsl::Vec3 position;
		sf::Glsl::Vec2 texture_coords;


		static VertexLayout GetLayout();
	};

   public:
	unsigned int size() const;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

   public:
    VertexArray VAO;
    VertexBuffer VBO;
	VertexBuffer IBO;

   private:
    size_t count_;
};
}  // namespace render
