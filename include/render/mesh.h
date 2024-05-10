#pragma once

#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <atomic>
#include <iostream>
#include <vector>

#include "data/shader.h"
#include "math/transform.h"
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
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
         MeshChange is_changeable = MeshChange::Disable)
        : vertices_(vertices),
          indices_(indices),
          changeable_(is_changeable),
          VBO(GL_ARRAY_BUFFER, (void*)vertices.data(), vertices.size() * sizeof(Vertex), is_changeable),
          IBO(GL_ELEMENT_ARRAY_BUFFER, (void*)indices.data(), indices.size() * sizeof(Vertex)) {
        VAO.AddBuffer(VBO, Vertex::GetLayout());
        VAO.AddBuffer(IBO);
    }

    const std::vector<Vertex>& Vertices() const { return vertices_; }
    const std::vector<unsigned int>& Indices() const { return indices_; }

    void SetVertex(int id, Vertex data) {
        if (changeable_ == MeshChange::Disable) {
            std::cout << "Try to change unchangeable mesh";
            exit(1);
        }
        VBO.Write(id * sizeof(Vertex), &data, sizeof(data));
        vertices_[id] = data;
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

    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
};
}  // namespace render
