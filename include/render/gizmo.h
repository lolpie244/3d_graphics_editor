#pragma once

#include <SFML/Graphics/Color.hpp>
#include <array>

#include "data/shader.h"
#include "events/propterties/draggable.h"
#include "math/transform.h"
#include "render/mesh.h"
#include "render/model.h"
#include "utils/uuid.h"

namespace render {

struct GizmoVertex : public Vertex<GizmoVertex> {
    glm::vec3 position;

    size_t Hash() const;
    VertexLayout Layout() const;
    bool operator==(const GizmoVertex& b) const { return position == b.position; }
    void Parse(const tinyobj::ObjReader& reader, tinyobj::index_t id);
};

class Gizmo : virtual public UUID, virtual public events::Draggable3D, virtual public math::Transform {
   public:
    Gizmo(const Mesh<GizmoVertex>::RawMesh& mesh);
    static std::unique_ptr<render::Gizmo> loadFromFile(const std::string& filename);

    void Draw(data::Shader& shader, Model* model);
    glm::vec3 VertexPosition(int id);

	const Mesh<GizmoVertex>& ModelMesh() const;

   private:
    float length_;
    Mesh<GizmoVertex> mesh_;
};
}  // namespace render
