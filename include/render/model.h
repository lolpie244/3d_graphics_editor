#pragma once

#include <SFML/Graphics/Transformable.hpp>

#include "data/shader.h"
#include "mesh.h"
#include "utils/uuid.h"
#include "vertex.h"

namespace render {
struct ModelVertex : public Vertex<ModelVertex> {
    glm::vec3 position;
    glm::vec2 texture_coord;
    glm::vec3 normal;
    glm::vec4 color{255, 255, 255, 255};

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
        Pending = 3,
    };
    typedef std::unordered_set<PickingTexture::Info, PickingTexture::Info::Hash> SelectedVertices;

   public:
    Model(const Mesh<ModelVertex>::RawMesh& mesh, MeshConfig config = MeshConfig());

    static std::unique_ptr<Model> loadFromFile(const std::string& filename, MeshConfig config = MeshConfig());

    void Draw(data::Shader& shader) const;
    void DrawPoints(data::Shader& shader) const;

    const ModelVertex Vertex(int id, unsigned int type) const;
    const std::vector<ModelVertex>& Vertices(unsigned int type) const;
	const Mesh<ModelVertex>& ModelMesh() const;

    void SetVertexPosition(int id, unsigned int type, glm::vec3 new_position);
    void SetVertexColor(int id, unsigned int type, glm::vec4 color);

    void AddFace(const std::vector<unsigned int>& face);

    int AddPenging(ModelVertex vertex);
    std::vector<unsigned int> RemovePendings(const std::vector<unsigned int> ids);

    void Triangulate(const SelectedVertices& vertices);

   private:
    Mesh<ModelVertex>* GetMesh(unsigned int type) { return type == DataType::Point ? &mesh_ : &pending_mesh_; }

   public:
    sf::Texture texture;

   private:
    Mesh<ModelVertex> mesh_;
    Mesh<ModelVertex> pending_mesh_;
};

typedef std::unordered_map<int, std::unique_ptr<render::Model>> ModelsList;
}  // namespace render
