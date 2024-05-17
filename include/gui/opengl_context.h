#pragma once

#include <glm/ext/vector_float2.hpp>

#include "events/propterties/draggable.h"
#include "events/propterties/scaleable.h"
#include "events/propterties/scrollable.h"
#include "gui/base.h"
#include "render/opengl/picking_texture.h"

namespace gui {
class OpenglContext : virtual public GuiElement,
                      virtual public events::Draggable,
                      virtual public events::Scaleable,
                      virtual public events::Scrollable {
   public:
    OpenglContext();
    OpenglContext(glm::vec3 position, glm::vec2 size);

    virtual sf::Rect<float> Rect() const override;
    virtual void Move(float x = 0, float y = 0, float z = -100) override;
    virtual void Resize(glm::vec2 size) override;
    virtual void SetLeftCorner(float x = 0, float y = 0, float z = -100);
    glm::vec2 LeftCorner() const;

   public:
    render::PickingTexture PickingTexture;

   private:
    void update_context();
};
}  // namespace gui
