#include "renderer/vertex_buffer.h"

#include <GL/glew.h>

#include <cstring>
#include <iostream>

namespace renderer {

VertexBuffer::VertexBuffer(unsigned int type, void* data, size_t size, unsigned int draw)
    : type_(type), draw_(draw), size_(size) {
    glGenBuffers(1, &buffer_id_);
    this->Allocate(data, size);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &buffer_id_); }

void VertexBuffer::Bind() const { glBindBuffer(type_, buffer_id_); }

void VertexBuffer::Unbind() const { glBindBuffer(type_, 0); }

void VertexBuffer::Allocate(void* data, size_t size) {
    Bind();
    this->size_ = size;
    glBufferData(type_, size, data, draw_);
    Unbind();
}

void VertexBuffer::Write(void* data, size_t size) {
    if (size != this->size_) {
        std::cout << "VertexBuffer::Write: different size; Realocate memory\n";
        Allocate(data, size);
        return;
    }
    Bind();
    void* ptr = glMapBuffer(type_, GL_WRITE_ONLY);
    memcpy(ptr, data, size);
    Unbind();
}

}  // namespace renderer
