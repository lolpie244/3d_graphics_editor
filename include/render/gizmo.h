#pragma once

#include <SFML/Graphics/Color.hpp>
#include <array>

#include "data/shader.h"
#include "math/transform.h"
#include "render/mesh.h"
#include "render/model.h"

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

class Gizmo : math::Transform {
   public:
    Gizmo(const std::vector<GizmoVertex>& vertices, const std::vector<unsigned int>& indices);
	void Draw(data::Shader& shader, Model* model);

   private:
	float length_;
	Mesh<GizmoVertex> mesh_;
};
}  // namespace render
