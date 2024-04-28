#pragma once

#include <tiny_obj_loader.h>

#include "model_loader.h"
#include "render/gizmo.h"

namespace data::parser {
template <>
struct Vec3Hash<render::ModelVertex> {
    size_t operator()(const render::ModelVertex& v) const {
        auto fhash = std::hash<float>{};
        return fhash(v.position.x) ^ (fhash(v.position.y) << 1) ^ (fhash(v.position.z) << 2) ^
               (fhash(v.texture_coords.x) << 3) ^ (fhash(v.texture_coords.y) << 4);
    }
};

template <>
inline render::ModelVertex Parse<render::ModelVertex>(const tinyobj::ObjReader& reader, tinyobj::index_t id) {
    render::ModelVertex vertex;
	auto& attrib = reader.GetAttrib();

    vertex.position = {attrib.vertices[3 * size_t(id.vertex_index) + 0],
                       attrib.vertices[3 * size_t(id.vertex_index) + 1],
                       attrib.vertices[3 * size_t(id.vertex_index) + 2]};

    if (id.texcoord_index >= 0) {
        vertex.texture_coords = {attrib.texcoords[2 * size_t(id.texcoord_index) + 0],
                                 1 - attrib.texcoords[2 * size_t(id.texcoord_index) + 1]};
    }

    return vertex;
}
}

namespace data {
inline std::unique_ptr<render::Model> loadModel(const std::string& filename, render::MeshChange is_changeable) {
	auto data = parser::loadFromFile<render::ModelVertex>(filename);
	return std::make_unique<render::Model>(data.first, data.second, is_changeable);
}

}  // namespace data::parser
