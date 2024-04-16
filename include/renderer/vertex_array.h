#pragma once
#include "renderer/vertex_buffer.h"
#include "vertex_layout.h"

namespace renderer {

class VertexArray {
   public:
	VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddBuffer(const VertexBuffer& vbo);
    void AddBuffer(const VertexBuffer& vbo, const VertexLayout& layout);

	void EnableAttributes();
	void DisableAttributes();

 private:
	unsigned int id_;
	unsigned int attribute_count_;
};
}  // namespace renderer
