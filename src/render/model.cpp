#include "render/model.h"

#include "stage/stage_manager.h"

namespace render {

std::atomic<int> Model::max_object_id{1};

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

void Model::Draw(data::Shader& shader) const {
    shader.setUniform("u_ObjectIndex", id_);
    shader.setUniform("u_Texture", texture);
    this->mesh_.Draw(GL_TRIANGLES, shader, this);
}
void Model::DrawPoints(data::Shader& shader) const {
    shader.setUniform("u_ObjectIndex", id_);
    this->mesh_.Draw(GL_POINTS, shader, this);
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

}  // namespace render
