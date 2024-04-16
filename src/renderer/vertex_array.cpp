#include "renderer/vertex_array.h"


namespace renderer {
	VertexArray::VertexArray() {
		glGenVertexArrays(1, &id_);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &id_);
	}

	void VertexArray::Bind() const {
		glBindVertexArray(id_);
	}

	void VertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void VertexArray::SetBuffer(const VertexBuffer& vbo, const VertexLayout& layout) {
		Bind();
		vbo.Bind();
		unsigned int offset = 0;

		for (int i = 0; i < layout.Elements().size(); i++) {
			glEnableVertexAttribArray(i);
			auto& element = layout.Elements()[i];
			glVertexAttribPointer(i, element.count, element.type, element.normalize, layout.Stride(), (const void*)offset);
			offset += element.count * element.size;
		}
		vbo.Unbind();
		Unbind();
	}

	void VertexArray::SetBuffer(const VertexBuffer& vbo, const VertexBuffer& ibo, const VertexLayout& layout) {
		Bind();
		ibo.Bind();
		SetBuffer(vbo, layout);
	}
}
