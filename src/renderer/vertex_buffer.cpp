#include "renderer/vertex_buffer.h"

#include <GL/glew.h>

#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>

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

void VertexBuffer::Write(int offset, void* data, size_t size) {
    if (offset + size > this->size_) {
        throw std::runtime_error("VertexBuffer::Write: size is larger that allocated\n");
    }

    Bind();
    glBufferSubData(type_, offset, size, data);
    Unbind();
}

}  // namespace renderer
