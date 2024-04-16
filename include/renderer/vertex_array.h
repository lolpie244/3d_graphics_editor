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

    void SetBuffer(const VertexBuffer& vbo, const VertexLayout& layout);
    void SetBuffer(const VertexBuffer& vbo, const VertexBuffer& ibo, const VertexLayout& layout);

 private:
	unsigned int id_;
};
}  // namespace renderer
