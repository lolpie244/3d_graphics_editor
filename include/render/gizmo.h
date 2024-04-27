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

struct GizmoVertex {
    glm::vec3 position;

    bool operator==(const GizmoVertex& b) const { return position == b.position; }

    static VertexLayout GetLayout() {
        VertexLayout result;
        result.Add<float>(3);  // position
        return result;
    }
};

class Gizmo : virtual public UUID, virtual public events::Draggable3D, virtual public math::Transform {
   public:
    Gizmo(const std::vector<GizmoVertex>& vertices, const std::vector<unsigned int>& indices);
    void Draw(data::Shader& shader, Model* model);

   private:
    float length_;
    Mesh<GizmoVertex> mesh_;
    std::array<sf::Color, 3> colors_{sf::Color::Green, sf::Color::Red, sf::Color::Blue};
};
}  // namespace render
