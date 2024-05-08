#pragma once

#include <SFML/Graphics/Transformable.hpp>

#include "data/shader.h"
#include "mesh.h"
#include "utils/uuid.h"
#include "vertex.h"

namespace render {
struct ModelVertex : public Vertex<ModelVertex> {
    glm::vec3 position;
    glm::vec2 texture_coords;
    glm::vec4 color {255, 255, 255, 255};

    size_t Hash() const;
    VertexLayout Layout() const;
    bool operator==(const ModelVertex& b) const;
    void Parse(const tinyobj::ObjReader& reader, tinyobj::index_t id);
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

	static std::unique_ptr<Model> loadFromFile(const std::string& filename, MeshChange is_changeable);

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
