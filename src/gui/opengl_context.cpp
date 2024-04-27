#include "gui/opengl_context.h"

#include <GL/glew.h>

#include "gui/base.h"
#include "math/vector2.h"
#include "render/opengl/picking_texture.h"
#include "stage/stage_manager.h"
#include "utils/settings.h"

namespace gui {
OpenglContext::OpenglContext() : PickingTexture(0, 0) {
    auto window_size = settings::DEFAULT_RESOLUTION;
    this->SetPosition(window_size.first / 2.0f, window_size.second / 2.0f, -100);
    this->Resize({window_size.first, window_size.second});
}

OpenglContext::OpenglContext(glm::vec3 position, math::Vector2f size) : PickingTexture(0, 0) {
    this->SetPosition(position);
    this->Resize(size);
}

sf::Rect<float> OpenglContext::Rect() const {
    auto left_corner = math::Vector2f(this->GetPosition()) - this->Size() / 2.0f;
    return {left_corner.x, left_corner.y, this->Size().x, this->Size().y};
}

void OpenglContext::Move(float x, float y, float z) {
    GuiElement::Move(x, y, z);
    update_context();
}

void OpenglContext::Resize(math::Vector2f size) {
    GuiElement::Resize(size);
    PickingTexture.Resize(size.x, size.y);
    update_context();
}

void OpenglContext::SetLeftCorner(float x, float y, float z) { GuiElement::Move(x - Rect().left, y - Rect().top, z); }

void OpenglContext::update_context() {
    auto rect = this->Rect();
    glViewport(rect.left, rect.top, rect.width, rect.height);
}
}  // namespace gui
