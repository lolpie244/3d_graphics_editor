#include "render/model.h"

#include "data/model_loader.h"
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

Model::Model(const std::vector<ModelVertex>& vertices, const std::vector<unsigned int>& indices,
             MeshChange is_changeable)
    : mesh_(vertices, indices, is_changeable) {
    glm::vec3 min = {INT_MAX, INT_MAX, INT_MAX}, max{-INT_MAX, -INT_MAX, -INT_MAX};
    for (auto& vertex : vertices) {
        min = {std::min(min.x, vertex.position.x), std::min(min.y, vertex.position.y),
               std::min(min.z, vertex.position.z)};

        max = {std::max(max.x, vertex.position.x), std::max(max.y, vertex.position.y),
               std::max(max.z, vertex.position.z)};
    }

    this->SetOrigin((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
}

std::unique_ptr<Model> Model::loadFromFile(const std::string& filename, MeshChange is_changeable) {
    auto data = data::parser::loadModelFromFile<render::ModelVertex>(filename);
    return std::make_unique<render::Model>(data.first, data.second, is_changeable);
}

void Model::Draw(data::Shader& shader) const {
    shader.setUniform("u_Data", DataType::Surface);
    shader.setUniform("u_ObjectId", Id());

    shader.setUniform("u_Texture", texture);
    this->mesh_.Draw(GL_TRIANGLES, shader, this);
}
void Model::DrawPoints(data::Shader& shader) const {
    shader.setUniform("u_Data", DataType::Point);
    shader.setUniform("u_ObjectId", Id());

    this->mesh_.Draw(GL_POINTS, shader, this);
    shader.setUniform("u_ObjectId", 0);
    this->mesh_.Draw(GL_LINES, shader, this);
}

const ModelVertex Model::Vertex(int id) const { return mesh_.Vertices()[id]; }

void Model::SetVertexPosition(int id, glm::vec3 new_position) {
    auto old_position = mesh_.Vertices()[id].position;
    for (int i = 0; i < mesh_.Vertices().size(); i++) {
        if (mesh_.Vertices()[i].position == old_position) {
            auto new_data = mesh_.Vertices()[i];
            new_data.position = new_position;
            mesh_.SetVertex(i, new_data);
        }
    }
}

void Model::SetVertexColor(int id, sf::Color color) {
    for (int i = 0; i < mesh_.Vertices().size(); i++) {
        if (mesh_.Vertices()[i].position == mesh_.Vertices()[id].position) {
            auto new_data = mesh_.Vertices()[i];
            new_data.color = {color.r, color.g, color.b, color.a};
            mesh_.SetVertex(i, new_data);
        }
    }
}

}  // namespace render
