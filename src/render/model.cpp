#include "render/model.h"

#include "data/model_loader.h"
#include "render/mesh.h"
#include "stage/stage_manager.h"

namespace render {

bool ModelVertex::operator==(const ModelVertex& b) const {
    return position == b.position && texture_coord == b.texture_coord && normal == b.normal && color == b.color;
}

VertexLayout ModelVertex::Layout() const {
    VertexLayout result;
    result.Add<float>(3);  // position
    result.Add<float>(2);  // texture coord
    result.Add<float>(3);  // normal
    result.Add<float>(4);  // color
    return result;
}

void ModelVertex::Parse(const tinyobj::ObjReader& reader, tinyobj::index_t id) {
    auto& attrib = reader.GetAttrib();

    position = {
        attrib.vertices[3 * size_t(id.vertex_index) + 0],
        attrib.vertices[3 * size_t(id.vertex_index) + 1],
        attrib.vertices[3 * size_t(id.vertex_index) + 2],
    };

    if (id.texcoord_index >= 0)
        texture_coord = {
            attrib.texcoords[2 * size_t(id.texcoord_index) + 0],
            1 - attrib.texcoords[2 * size_t(id.texcoord_index) + 1],
        };

    if (id.normal_index >= 0)
        normal = {
            attrib.normals[3 * size_t(id.normal_index) + 0],
            attrib.normals[3 * size_t(id.normal_index) + 1],
            attrib.normals[3 * size_t(id.normal_index) + 2],
        };
}

size_t ModelVertex::Hash() const {
    auto fhash = std::hash<float>{};
    return fhash(position.x) ^ (fhash(position.y) << 1) ^ (fhash(position.z) << 2) ^ (fhash(texture_coord.x) << 3) ^
           (fhash(texture_coord.y) << 4) ^ (fhash(normal.x) << 5) ^ (fhash(normal.y) << 6) ^ (fhash(normal.z) << 7);
}

Model::Model(const Mesh<ModelVertex>::RawMesh& mesh, MeshConfig config)
    : mesh_(mesh, config), pending_mesh_({}, MeshConfig({.changeable = MeshConfig::Dynamic, .triangulate = false})) {
    glm::vec3 min = {INT_MAX, INT_MAX, INT_MAX}, max{-INT_MAX, -INT_MAX, -INT_MAX};
    for (auto& vertex : mesh.vertices) {
        min = {std::min(min.x, vertex.position.x), std::min(min.y, vertex.position.y),
               std::min(min.z, vertex.position.z)};

        max = {std::max(max.x, vertex.position.x), std::max(max.y, vertex.position.y),
               std::max(max.z, vertex.position.z)};
    }

    this->SetOrigin((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
}

std::unique_ptr<Model> Model::loadFromFile(const std::string& filename, MeshConfig config) {
    auto data = data::parser::loadModelFromFile<render::ModelVertex>(filename);
    return std::make_unique<render::Model>(data, config);
}

void Model::Draw(data::Shader& shader) const {
    shader.setUniform("u_Data", DataType::Surface);
    shader.setUniform("u_ObjectId", Id());

    shader.setUniform("u_Texture", texture);
    this->mesh_.Draw(GL_TRIANGLES, shader, this);
}
void Model::DrawPoints(data::Shader& shader) const {
    shader.setUniform("u_ObjectId", 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    this->mesh_.Draw(GL_TRIANGLES, shader, this);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader.setUniform("u_ObjectId", Id());
    shader.setUniform("u_Data", DataType::Point);
    this->mesh_.Draw(GL_POINTS, shader, this);

	glClear(GL_DEPTH_BUFFER_BIT);
    shader.setUniform("u_Data", DataType::Pending);
    this->pending_mesh_.Draw(GL_POINTS, shader, this);
}

const ModelVertex Model::Vertex(int id, unsigned int type) const {
    if (type == DataType::Pending)
        return pending_mesh_.Vertices()[id];
    return mesh_.Vertices()[id];
}

void Model::SetVertexPosition(int id, unsigned int type, glm::vec3 new_position) {
    auto old_position = Vertex(id, type).position;
	auto* mesh = type == DataType::Point ? &mesh_ : &pending_mesh_;

    for (int i = 0; i < mesh->Vertices().size(); i++) {
        if (mesh->Vertices()[i].position == old_position) {
            auto new_data = mesh->Vertices()[i];
            new_data.position = new_position;
            mesh->SetVertex(i, new_data);
        }
    }
}

void Model::SetVertexColor(int id, unsigned int type, sf::Color color) {
	auto* mesh = type == DataType::Point ? &mesh_ : &pending_mesh_;

    for (int i = 0; i < mesh->Vertices().size(); i++) {
        if (mesh->Vertices()[i].position == mesh->Vertices()[id].position) {
            auto new_data = mesh->Vertices()[i];
            new_data.color = {color.r, color.g, color.b, color.a};
            mesh->SetVertex(i, new_data);
        }
    }
}

int Model::AddPenging(ModelVertex vertex) {
    unsigned int vertex_id = pending_mesh_.Vertices().size();
    pending_mesh_.AddVertices({vertex});
    pending_mesh_.AddIndices({vertex_id});
    std::cout << vertex_id << '\n';
    return vertex_id;
}

}  // namespace render
