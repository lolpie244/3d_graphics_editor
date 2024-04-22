#pragma once

#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <atomic>
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

	enum Change {
		Disable = GL_STATIC_DRAW,
		Enable = GL_DYNAMIC_DRAW,
	};

   public:
    unsigned int size() const;
    int Id() const;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Change is_changeable = Change::Disable);

	const Vertex GetVertex(int id) const;
	const Vertex GetVertexByIndex(int id) const;

	void SetVertex(int id, Vertex data);
	void SetVertexPosition(int id, glm::vec3 position);

   public:
    VertexArray VAO;
    VertexBuffer VBO;
    VertexBuffer IBO;

    sf::Texture texture;

   private:
    size_t count_;
	int id_ = max_object_id++;

	Change changeable_;

	std::vector<Vertex> vertices_;
	std::vector<unsigned int> indices_;

    static std::atomic<int> max_object_id;
};
}  // namespace render
