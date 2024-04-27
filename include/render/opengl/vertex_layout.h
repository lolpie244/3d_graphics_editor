#pragma once

#include <GL/glew.h>

#include <vector>

namespace render {
class VertexLayout {
    struct Element {
        unsigned int type;
        unsigned int count;
        unsigned int size;
        unsigned int normalize;
    };

   public:
    template <typename T>
    void Add(unsigned int count) {
        static_assert(false, "Incorrect type");
    }

    const std::vector<Element>& Elements() const { return elements; }

    const unsigned int Stride() const { return stride; }

   private:
    std::vector<Element> elements;
    unsigned int stride = 0;
};

template <>
inline void VertexLayout::Add<float>(unsigned int count) {
    elements.emplace_back(GL_FLOAT, count, sizeof(float), GL_FALSE);
    stride += count * sizeof(float);
}

}  // namespace render
