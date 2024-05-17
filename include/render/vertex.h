#pragma once

#include <tiny_obj_loader.h>

#include <stdexcept>

#include "render/opengl/vertex_layout.h"

namespace render {
template <typename Derived>
struct Vertex {
    size_t Hash() const {
        throw std::runtime_error("please, override this method");
        return 0;
    }
    VertexLayout Layout() const {
        throw std::runtime_error("please, override this method");
        return VertexLayout();
    }
    void Parse(const tinyobj::ObjReader& reader, tinyobj::index_t id) {
        throw std::runtime_error("please, override this method");
    }
    bool operator==(const Derived& b) const {
        throw std::runtime_error("please, override this method");
        return false;
    }

    static VertexLayout GetLayout() { return Derived{}.Layout(); }
};
}  // namespace render
