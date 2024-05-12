#pragma once

#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <algorithm>
#include <atomic>
#include <iostream>
#include <iterator>
#include <vector>

#include "data/shader.h"
#include "math/transform.h"
#include "math/triangulation.h"
#include "render/opengl/vertex_array.h"
#include "render/opengl/vertex_buffer.h"
#include "render/opengl/vertex_layout.h"
#include "stage/stage_manager.h"

namespace render {

struct MeshConfig {
    enum Changeable {
        Static = GL_STATIC_DRAW,
        Dynamic = GL_DYNAMIC_DRAW,
    };

    Changeable changeable = Changeable::Static;
    bool triangulate = true;
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
    Mesh(const RawMesh& mesh, MeshConfig config)
        : raw_mesh_(mesh),
          config_(config),
          VBO(GL_ARRAY_BUFFER, (void*)mesh.vertices.data(), mesh.vertices.capacity() * sizeof(Vertex),
              config.changeable),
          IBO(GL_ELEMENT_ARRAY_BUFFER, nullptr, 0, config.changeable) {
        VAO.AddBuffer(VBO, Vertex::GetLayout());
        VAO.AddBuffer(IBO);

        int current_index = 0;

        if (!config_.triangulate) {
            IBO.Allocate(raw_mesh_.indices.data(), raw_mesh_.indices.capacity() * sizeof(unsigned int));
            return;
        }

        for (auto size : mesh.face_vertices) {
            std::vector<glm::vec3> positions(size);
            for (int i = 0; i < size; i++) positions[i] = raw_mesh_.vertices[mesh.indices[current_index + i]].position;

            for (auto new_index : math::EarCuttingTriangulate::Instance().Traingulate(positions)) {
                triangulated_indices_.push_back(mesh.indices[current_index + new_index]);
            }

            current_index += size;
        }
        IBO.Allocate(triangulated_indices_.data(), triangulated_indices_.capacity() * sizeof(unsigned int));
    }

    const std::vector<Vertex>& Vertices() const { return raw_mesh_.vertices; }
    const std::vector<unsigned int>& Indices() const {
        if (config_.triangulate)
            return triangulated_indices_;
        return raw_mesh_.indices;
    }

    void SetVertex(int id, Vertex data) {
        if (config_.changeable == MeshConfig::Static) {
            std::cout << "Try to change unchangeable mesh";
            exit(1);
        }
        VBO.Write(id * sizeof(Vertex), &data, sizeof(data));
        raw_mesh_.vertices[id] = data;

        if (!config_.triangulate)
            return;

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
            for (int i = 0; i < result.size(); i++)
                triangulated_indices_[real_index + i] = raw_mesh_.indices[result[i] + face_sum];

            IBO.Write(real_index * sizeof(unsigned int), &triangulated_indices_[real_index],
                      result.size() * sizeof(unsigned int));
        }
    }
    void AddVertices(const std::vector<Vertex>& data) {
        if (config_.changeable == MeshConfig::Static) {
            std::cout << "Try to change unchangeable mesh";
            exit(1);
        }

        int old_capacity = raw_mesh_.vertices.capacity();
        int start_id = raw_mesh_.vertices.size();

        for (auto& vertex : data) raw_mesh_.vertices.push_back(vertex);

        if (raw_mesh_.vertices.capacity() == old_capacity)
            VBO.Write(sizeof(Vertex) * start_id, &raw_mesh_.vertices[start_id], sizeof(Vertex) * data.size());
        else
            VBO.Allocate(raw_mesh_.vertices.data(), raw_mesh_.vertices.capacity() * sizeof(Vertex));
    }

    void AddIndices(const std::vector<unsigned int>& data) {
        if (config_.changeable == MeshConfig::Static) {
            std::cout << "Try to change unchangeable mesh";
            exit(1);
        }

        auto& indices = this->triangulated_indices_;
        if (!config_.triangulate) {
            indices = raw_mesh_.indices;
        }

        int old_capacity = indices.capacity();
        int start_id = indices.size();

        for (auto index : data) indices.push_back(index);

        if (old_capacity == indices.capacity())
            IBO.Write(start_id * sizeof(unsigned int), &indices[start_id], data.size() * sizeof(unsigned int));
        else
            IBO.Allocate(indices.data(), indices.capacity() * sizeof(Vertex));
    }

    void AddFace(const std::vector<unsigned int>& face) {
        raw_mesh_.face_vertices.push_back(face);
        if (!config_.triangulate) {
            AddIndices(face);
            return;
        }
        int face_sum = sum(raw_mesh_.face_vertices.begin(), raw_mesh_.face_vertices.end());
        int face_idx = raw_mesh_.face_vertices.size();

        raw_mesh_.face_vertices.push_back(face.size());
        std::copy(face.begin(), face.end(), std::back_inserter(raw_mesh_.indices));

        int old_capacity = triangulated_indices_.capacity();
        int start_size = triangulated_indices_.size();

        std::vector<glm::vec3> positions(raw_mesh_.face_vertices[face_idx]);
        for (int i = 0; i < raw_mesh_.face_vertices[face_idx]; i++)
            positions[i] = raw_mesh_.vertices[raw_mesh_.indices[face_sum + i]].position;

        auto result = math::EarCuttingTriangulate::Instance().Traingulate(positions);
        for (int i = 0; i < result.size(); i++) result[i] = raw_mesh_.indices[result[i] + face_sum];

        AddIndices(result);
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
    MeshConfig config_;

    RawMesh raw_mesh_;
    std::vector<unsigned int> triangulated_indices_;
};
}  // namespace render
