#include "gui/opengl_context.h"

#include <GL/glew.h>

#include "gui/base.h"
#include "render/opengl/picking_texture.h"
#include "stage/stage_manager.h"
#include "utils/settings.h"

namespace gui {
OpenglContext::OpenglContext() : PickingTexture(0, 0) {
    auto window_size = settings::DEFAULT_RESOLUTION;
    this->SetPosition(window_size.x / 2.0f, window_size.y / 2.0f, -100);
    this->Resize({window_size.x, window_size.y});
}

OpenglContext::OpenglContext(glm::vec3 position, glm::vec2 size) : PickingTexture(0, 0) {
    this->SetPosition(position);
    this->Resize(size);
}

sf::Rect<float> OpenglContext::Rect() const {
    auto left_corner = glm::vec2(this->GetPosition()) - this->Size() / 2.0f;
    return {left_corner.x, left_corner.y, this->Size().x, this->Size().y};
}

void OpenglContext::Move(float x, float y, float z) {
    GuiElement::Move(x, y, z);
    update_context();
}

void OpenglContext::Resize(glm::vec2 size) {
    GuiElement::Resize(size);
    update_context();
}

void OpenglContext::SetLeftCorner(float x, float y, float z) { GuiElement::Move(x - Rect().left, y - Rect().top, z); }
glm::vec2 OpenglContext::LeftCorner() const { return {Rect().left, Rect().top}; }

void OpenglContext::update_context() {
    auto rect = this->Rect();
    PickingTexture.Resize(rect.width, rect.height);
    glViewport(rect.left, settings::DEFAULT_RESOLUTION.y - rect.height, rect.width,
               settings::DEFAULT_RESOLUTION.y - rect.top);
}
}  // namespace gui
