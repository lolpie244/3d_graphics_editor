#pragma once

#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <atomic>
#include <iostream>
#include <vector>

#include "data/shader.h"
#include "math/transform.h"
#include "math/triangulation.h"
#include "render/opengl/vertex_array.h"
#include "render/opengl/vertex_buffer.h"
#include "render/opengl/vertex_layout.h"
#include "stage/stage_manager.h"

namespace render {

enum MeshChange {
    Disable = GL_STATIC_DRAW,
    Enable = GL_DYNAMIC_DRAW,
};

template <typename Vertex>
class Mesh {
   public:
    struct RawMesh {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<unsigned char> face_vertices;
    };

   public:
    Mesh(const RawMesh& mesh, MeshChange is_changeable = MeshChange::Disable)
        : raw_mesh_(mesh),
          changeable_(is_changeable),
          VBO(GL_ARRAY_BUFFER, (void*)mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex), is_changeable),
          IBO(GL_ELEMENT_ARRAY_BUFFER, nullptr, 0, is_changeable) {
        VAO.AddBuffer(VBO, Vertex::GetLayout());
        VAO.AddBuffer(IBO);

        int current_index = 0;

        for (auto size : mesh.face_vertices) {
            std::vector<glm::vec3> positions(size);
            for (int i = 0; i < size; i++) positions[i] = raw_mesh_.vertices[mesh.indices[current_index + i]].position;

            for (auto new_index : math::EarCuttingTriangulate::Instance().Traingulate(positions)) {
                indices_.push_back(mesh.indices[current_index + new_index]);
            }

            current_index += size;
        }
        IBO.Allocate(indices_.data(), indices_.size() * sizeof(unsigned int));
    }

    const std::vector<Vertex>& Vertices() const { return raw_mesh_.vertices; }
    const std::vector<unsigned int>& Indices() const { return indices_; }

    void SetVertex(int id, Vertex data) {
        if (changeable_ == MeshChange::Disable) {
            std::cout << "Try to change unchangeable mesh";
            exit(1);
        }
        VBO.Write(id * sizeof(Vertex), &data, sizeof(data));
        raw_mesh_.vertices[id] = data;

        int face_idx = 0;
        int face_sum = 0;
        int real_index = 0;

        for (int i = 0; i < raw_mesh_.indices.size(); i++) {
            if (face_sum + raw_mesh_.face_vertices[face_idx] <= i) {
                face_sum += raw_mesh_.face_vertices[face_idx];
                real_index += (raw_mesh_.face_vertices[face_idx] - 2) * 3;
                face_idx++;
            }

            if (raw_mesh_.indices[i] != id)
                continue;

            std::vector<glm::vec3> positions(raw_mesh_.face_vertices[face_idx]);

            for (int i = 0; i < raw_mesh_.face_vertices[face_idx]; i++)
                positions[i] = raw_mesh_.vertices[raw_mesh_.indices[face_sum + i]].position;

            auto result = math::EarCuttingTriangulate::Instance().Traingulate(positions);
            for (int i = 0; i < result.size(); i++) indices_[real_index + i] = raw_mesh_.indices[result[i] + face_sum];

            IBO.Write(real_index * sizeof(unsigned int), &indices_[real_index], result.size() * sizeof(unsigned int));
        }
    }
    void Draw(unsigned int type, data::Shader& shader, const glm::mat4& transform) const {
        shader.setUniform("u_Model", transform);
        Draw(type, shader);
    }

    void Draw(unsigned int type, data::Shader& shader, const math::Transform* transform) const {
        shader.setUniform("u_Model", transform->GetTransformation());
        Draw(type, shader);
    }

    void Draw(unsigned int type, data::Shader& shader) const {
        auto& camera = stage::StageManager::Instance().Camera();
        shader.setUniform("u_Projection", camera->ProjectionMatrix());
        shader.setUniform("u_View", camera->GetTransformation());

        sf::Shader::bind(&shader);
        VAO.Bind();
        glDrawElements(type, Indices().size(), GL_UNSIGNED_INT, NULL);
        VAO.Unbind();
    }

   protected:
    VertexArray VAO;
    VertexBuffer VBO;
    VertexBuffer IBO;

   private:
    MeshChange changeable_;

    RawMesh raw_mesh_;
    std::vector<unsigned int> indices_;
};
}  // namespace render
