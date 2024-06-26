#pragma once

#include <cstddef>
#include <GL/glew.h>

namespace render {

class VertexBuffer {
   public:
    VertexBuffer(unsigned int type, void* data, size_t size, unsigned int draw = GL_STATIC_DRAW);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

	void Allocate(void* data, size_t size);
	void Write(int offset, void* data, size_t size);

   private:
    unsigned int buffer_id_;
	unsigned int draw_;
	unsigned int type_;
	size_t size_;

	int count;
};
}  // namespace render
