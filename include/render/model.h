#pragma once

#include <SFML/Graphics/Transformable.hpp>

#include "data/shader.h"
#include "events/propterties/clickable.h"
#include "events/propterties/draggable.h"
#include "math/vector2.h"
#include "mesh.h"
#include "render/opengl/picking_texture.h"
#include "utils/uuid.h"

namespace render {
struct ModelVertex {
    glm::vec3 position;
    glm::vec2 texture_coords;

    bool operator==(const ModelVertex& b) const { return position == b.position && texture_coords == b.texture_coords; }

    static VertexLayout GetLayout() {
        VertexLayout result;
        result.Add<float>(3);  // position
        result.Add<float>(2);  // texture coords
        return result;
    }
};

class Model : virtual public UUID, virtual public math::Transform, virtual public events::Draggable3D {
   public:
    Model(const std::vector<ModelVertex>& vertices, const std::vector<unsigned int>& indices,
          MeshChange is_changeable = MeshChange::Disable);

    void Draw(data::Shader& shader) const;
    void DrawPoints(data::Shader& shader) const;
    // bool Contains(math::Vector2f point) override;

    const ModelVertex Vertex(int id) const;
    void SetVertexPosition(int id, glm::vec3 new_position);

   public:
    sf::Texture texture;

   private:
    Mesh<ModelVertex> mesh_;
};

}  // namespace render
