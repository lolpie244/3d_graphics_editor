#pragma once

#include <tiny_obj_loader.h>

#include "model_loader.h"
#include "render/gizmo.h"

namespace data::parser {

template <>
inline render::GizmoVertex Parse(const tinyobj::ObjReader& reader, tinyobj::index_t id) {
    render::GizmoVertex vertex;
	auto& attrib = reader.GetAttrib();

    vertex.position = {attrib.vertices[3 * size_t(id.vertex_index) + 0],
                       attrib.vertices[3 * size_t(id.vertex_index) + 1],
                       attrib.vertices[3 * size_t(id.vertex_index) + 2]};

    return vertex;
}

template <>
struct Vec3Hash<render::GizmoVertex> {
    size_t operator()(const render::GizmoVertex& v) const {
        auto fhash = std::hash<float>{};
        return fhash(v.position.x) ^ (fhash(v.position.y) << 1) ^ (fhash(v.position.z) << 2);
    }
};
}  // namespace data::parser

namespace data {
inline std::shared_ptr<render::Gizmo> loadGizmo(const std::string& filename) {
    auto data = parser::loadFromFile<render::GizmoVertex>(filename);
    return std::make_shared<render::Gizmo>(data.first, data.second);
}
}  // namespace data
