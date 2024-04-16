#include "renderer/vertex_array.h"

namespace renderer {
VertexArray::VertexArray() { glGenVertexArrays(1, &id_); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &id_); }

void VertexArray::Bind() const { glBindVertexArray(id_); }

void VertexArray::Unbind() const { glBindVertexArray(0); }

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexLayout& layout) {
    Bind();
    vbo.Bind();
    unsigned int offset = 0;
    attribute_count_ = layout.Elements().size();

    for (int i = 0; i < attribute_count_; i++) {
        glEnableVertexAttribArray(i);
        auto& attribute = layout.Elements()[i];
        glVertexAttribPointer(i, attribute.count, attribute.type, attribute.normalize, layout.Stride(),
                              (const void*)offset);
        offset += attribute.count * attribute.size;
    }
    vbo.Unbind();
    Unbind();
}

void VertexArray::AddBuffer(const VertexBuffer& vbo) {
    Bind();
    vbo.Bind();
    Unbind();
}

void VertexArray::EnableAttributes() {
    Bind();
    for (int i = 0; i < attribute_count_; i++) glEnableVertexAttribArray(i);
    Unbind();
}

void VertexArray::DisableAttributes() {
    Bind();
    for (int i = 0; i < attribute_count_; i++) glDisableVertexAttribArray(i);
    Unbind();
}
}  // namespace renderer
