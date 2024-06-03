#pragma once

#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

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
    struct Face {
        std::vector<unsigned int> indices;
        int indices_start = 0;
    };

    struct RawMesh {
        std::vector<Vertex> vertices;
        std::vector<Face> faces;
    };

   public:
    Mesh(MeshConfig config)
        : raw_mesh_({}),
          config_(config),
          VBO(GL_ARRAY_BUFFER, nullptr, 0, config.changeable),
          IBO(GL_ELEMENT_ARRAY_BUFFER, nullptr, 0, config.changeable) {
        VAO.AddBuffer(VBO, Vertex::GetLayout());
        VAO.AddBuffer(IBO);
    }

    Mesh(const RawMesh& mesh, MeshConfig config)
        : raw_mesh_(mesh),
          config_(config),
          VBO(GL_ARRAY_BUFFER, (void*)mesh.vertices.data(), mesh.vertices.capacity() * sizeof(Vertex),
              config.changeable),
          IBO(GL_ELEMENT_ARRAY_BUFFER, nullptr, 0, config.changeable) {
        VAO.AddBuffer(VBO, Vertex::GetLayout());
        VAO.AddBuffer(IBO);

        if (!config_.triangulate) {
            return;
        }

        for (int face_id = 0; face_id < raw_mesh_.faces.size(); face_id++) {
            Mesh::Face& face = raw_mesh_.faces[face_id];
            std::vector<glm::vec3> positions(face.indices.size());

            for (int i = 0; i < face.indices.size(); i++) {
                faces_of_vertices_[face.indices[i]].push_back(face_id);
                positions[i] = raw_mesh_.vertices[face.indices[i]].position;
            }

            face.indices_start = triangulated_indices_.size();

            for (auto id : math::EarCuttingTriangulate::Instance().Traingulate(positions))
                triangulated_indices_.push_back(face.indices[id]);
        }

        IBO.Allocate(triangulated_indices_.data(), triangulated_indices_.capacity() * sizeof(unsigned int));
    }

    const std::vector<Vertex>& Vertices() const { return raw_mesh_.vertices; }
    const std::vector<unsigned int>& Indices() const { return triangulated_indices_; }

    void RemoveVertex(int id) {
        // auto* indices = &this->triangulated_indices_;
        // if (!config_.triangulate) {
        //     indices = &raw_mesh_.indices;
        // }

        triangulated_indices_.erase(std::remove(triangulated_indices_.begin(), triangulated_indices_.end(), id),
                                    triangulated_indices_.end());
        IBO.Write(0, triangulated_indices_.data(), triangulated_indices_.size() * sizeof(unsigned int));

        raw_mesh_.vertices.erase(raw_mesh_.vertices.begin() + id);
    }

    void SetVertex(int id, Vertex data) {
        if (config_.changeable == MeshConfig::Static) {
            std::cout << "Try to change unchangeable mesh";
            exit(1);
        }
        VBO.Write(id * sizeof(Vertex), &data, sizeof(data));
        raw_mesh_.vertices[id] = data;
    }

    void Triangulate(const std::vector<unsigned int>& ids) {
        for (auto id : ids) {
            for (int face_id : faces_of_vertices_[id]) {
                Mesh::Face& face = raw_mesh_.faces[face_id];
                std::vector<glm::vec3> positions(face.indices.size());

                for (int i = 0; i < face.indices.size(); i++)
                    positions[i] = raw_mesh_.vertices[face.indices[i]].position;

                auto triangulated = math::EarCuttingTriangulate::Instance().Traingulate(positions);
                for (int i = 0; i < triangulated.size(); i++)
                    triangulated_indices_[face.indices_start + i] = face.indices[triangulated[i]];

                IBO.Write(face.indices_start * sizeof(unsigned int), &triangulated_indices_[face.indices_start],
                          triangulated.size() * sizeof(unsigned int));
            }
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
            VBO.Write(sizeof(Vertex) * start_id, &raw_mesh_.vertices[start_id], data.size() * sizeof(Vertex));
        else
            VBO.Allocate(raw_mesh_.vertices.data(), raw_mesh_.vertices.capacity() * sizeof(Vertex));
    }

    void AddIndices(const std::vector<unsigned int>& data) {
        if (config_.changeable == MeshConfig::Static) {
            std::cout << "Try to change unchangeable mesh";
            exit(1);
        }

        int old_capacity = triangulated_indices_.capacity();
        int start_id = triangulated_indices_.size();

        for (auto index : data) triangulated_indices_.push_back(index);

        if (old_capacity == triangulated_indices_.capacity())
            IBO.Write(start_id * sizeof(unsigned int), (void*)data.data(), data.size() * sizeof(unsigned int));
        else
            IBO.Allocate(triangulated_indices_.data(), triangulated_indices_.capacity() * sizeof(unsigned int));
    }

    void AddFace(const std::vector<unsigned int>& face) {
        if (!config_.triangulate) {
            AddIndices(face);
            return;
        }

        Mesh::Face new_face{
            .indices = face,
            .indices_start = (int)triangulated_indices_.size(),
        };
        std::vector<glm::vec3> positions(new_face.indices.size());

        for (int i = 0; i < new_face.indices.size(); i++)
            positions[i] = raw_mesh_.vertices[new_face.indices[i]].position;

        auto result = math::EarCuttingTriangulate::Instance().Traingulate(positions);
        for (int i = 0; i < result.size(); i++) result[i] = new_face.indices[result[i]];
        AddIndices(result);
    }

    void Draw(unsigned int type, data::Shader& shader, const glm::mat4& transform) const {
        shader.setUniform("u_Model", transform);
        Draw(type, shader);
    }

    void Draw(unsigned int type, data::Shader& shader, const math::MatrixTransform* transform) const {
        shader.setUniform("u_Model", transform->GetTransformation());
        Draw(type, shader);
    }

    void Draw(unsigned int type, data::Shader& shader) const {
        auto& camera = stage::StageManager::Instance().Camera();
        shader.setUniform("u_Projection", camera->ProjectionMatrix());
        shader.setUniform("u_View", camera->GetTransformation());

        sf::Shader::bind(&shader);
        VAO.Bind();
        glDrawElements(type, triangulated_indices_.size(), GL_UNSIGNED_INT, NULL);
        VAO.Unbind();
    }

    std::pair<glm::vec3, glm::vec3> MeshBox() const {
        glm::vec3 min = {INT_MAX, INT_MAX, INT_MAX}, max{-INT_MAX, -INT_MAX, -INT_MAX};
        for (auto& vertex : raw_mesh_.vertices) {
            min = {std::min(min.x, vertex.position.x), std::min(min.y, vertex.position.y),
                   std::min(min.z, vertex.position.z)};

            max = {std::max(max.x, vertex.position.x), std::max(max.y, vertex.position.y),
                   std::max(max.z, vertex.position.z)};
        }
        return {min, max};
    }

	RawMesh GetRawMesh() const {
		return raw_mesh_;
	}

   protected:
    VertexArray VAO;
    VertexBuffer VBO;
    VertexBuffer IBO;

   private:
    MeshConfig config_;

    std::vector<unsigned int> triangulated_indices_;
    std::unordered_map<unsigned int, std::list<int>> faces_of_vertices_;
    RawMesh raw_mesh_;
};
}  // namespace render
