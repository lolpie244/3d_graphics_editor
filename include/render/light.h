#pragma once

#include "data/shader.h"
#include "events/propterties/clickable.h"
#include "events/propterties/draggable.h"
#include "math/transform.h"
#include "render/mesh.h"
#include "render/model_for_gizmo.h"
#include "render/vertex.h"
#include "utils/uuid.h"
namespace render {
struct LightVertex : Vertex<LightVertex> {
    glm::vec3 position;
    VertexLayout Layout() const;
};

class Light : virtual public UUID, virtual public events::Clickable3D, virtual public GizmoSupport {
   public:
    struct LightData {
        glm::vec3 color;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

   public:
    Light(LightData data);

    std::pair<glm::vec3, glm::vec3> MeshBox() const override;
    void Draw(data::Shader& shader);
    void Apply(int id, data::Shader& shader);

	void Scale(float x = 1, float y = 1, float z = 1) override;

   public:
    LightData Data;

   private:
    Mesh<LightVertex> mesh_;
};

typedef std::unordered_map<int, std::unique_ptr<render::Light>> LightList;

}  // namespace render
