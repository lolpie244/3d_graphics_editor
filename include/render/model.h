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
    glm::vec4 color{255, 255, 255, 255};

    bool operator==(const ModelVertex& b) const {
        return position == b.position && texture_coords == b.texture_coords && color == b.color;
    }

    static VertexLayout GetLayout() {
        VertexLayout result;
        result.Add<float>(3);  // position
        result.Add<float>(2);  // texture coords
        result.Add<float>(4);  // color
        return result;
    }
};

class Model : virtual public UUID, virtual public math::Transform, virtual public events::Draggable3D {
   public:
    enum DataType {
        Point = 1,
        Surface = 2,
    };

   public:
    Model(const std::vector<ModelVertex>& vertices, const std::vector<unsigned int>& indices,
          MeshChange is_changeable = MeshChange::Disable);

    void Draw(data::Shader& shader) const;
    void DrawPoints(data::Shader& shader) const;

    const ModelVertex Vertex(int id) const;
    void SetVertexPosition(int id, glm::vec3 new_position);
    void SetVertexColor(int id, sf::Color color);

   public:
    sf::Texture texture;

   private:
    Mesh<ModelVertex> mesh_;
};

typedef std::unordered_map<int, std::unique_ptr<render::Model>> ModelsList;
}  // namespace render
