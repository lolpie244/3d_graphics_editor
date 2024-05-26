#pragma once

#include <SFML/Graphics/Color.hpp>
#include <array>

#include "data/model_loader.h"
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

class Gizmo : virtual public UUID, virtual public events::Draggable3D, virtual public math::LocalModelTransform {
   public:
    Gizmo(const Mesh<GizmoVertex>::RawMesh& mesh);

    template <typename T>
    static std::unique_ptr<render::Gizmo> loadFromFile(const std::string& filename) {
        return std::make_unique<T>(data::parser::loadModelFromFile<GizmoVertex>(filename));
    }

    void Draw(data::Shader& shader);

    void SetModel(render::Model* model);
    render::Model* GetModel() { return current_model_; }

    void Reset();
    virtual void MousePress(){};
    virtual void MouseMove(glm::vec2 mouse_position, glm::vec3 mouse_moved, unsigned int axis) = 0;

   protected:
    glm::vec3 Normal(unsigned int axis) const;

   protected:
    render::Model* current_model_ = nullptr;

   private:
    float length_;
    Mesh<GizmoVertex> mesh_;
};

class TranslateGizmo : public Gizmo {
   public:
    using Gizmo::Gizmo;

    void MouseMove(glm::vec2 mouse_position, glm::vec3 mouse_moved, unsigned int axis) override;
};

class ScaleGizmo : public Gizmo {
   public:
    using Gizmo::Gizmo;

    void MouseMove(glm::vec2 mouse_position, glm::vec3 mouse_moved, unsigned int axis) override;
};

class RotateGizmo : public Gizmo {
   public:
    using Gizmo::Gizmo;

    void MousePress() override;
    void MouseMove(glm::vec2 mouse_position, glm::vec3 mouse_moved, unsigned int axis) override;

   private:
    glm::vec3 last_position = {-1, -1, -1};
};
}  // namespace render
