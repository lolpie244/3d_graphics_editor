#pragma once

#include "events/propterties/draggable.h"
#include "events/propterties/scaleable.h"
#include "gui/base.h"

namespace gui {
class OpenglContext : virtual public GuiElement, virtual public events::Draggable, virtual public events::Scaleable {
   public:
	OpenglContext();
	OpenglContext(glm::vec3 position, math::Vector2f size);

    virtual sf::Rect<float> Rect() const override;
    virtual void Move(float x = 0, float y = 0, float z = 0) override;
    virtual void Resize(math::Vector2f size) override;
    virtual void SetLeftCorner(float x = 0, float y = 0, float z = 0);

 private:
	void update_context();
};
}  // namespace gui
