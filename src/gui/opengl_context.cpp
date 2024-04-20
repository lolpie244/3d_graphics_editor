#include "gui/opengl_context.h"

#include <GL/glew.h>

#include "gui/base.h"
#include "math/vector2.h"
#include "stage/stage_manager.h"
#include "utils/settings.h"

namespace gui {
OpenglContext::OpenglContext() {
    auto window_size = settings::DEFAULT_RESOLUTION;
    this->SetPosition(window_size.first / 2.0f, window_size.second / 2.0f);
    this->Resize({window_size.first, window_size.second});
}

OpenglContext::OpenglContext(glm::vec3 position, math::Vector2f size) {
    this->SetPosition(position);
    this->Resize(size);
}

sf::Rect<float> OpenglContext::Rect() const {
    auto left_corner = math::Vector2f(this->Position()) - this->Size() / 2.0f;
    return {left_corner.x, left_corner.y, this->Size().x, this->Size().y};
}

void OpenglContext::Move(float x, float y, float z) {
    GuiElement::Move(x, y, z);
    update_context();
}

void OpenglContext::Resize(math::Vector2f size) {
    GuiElement::Resize(size);
    update_context();
}

void OpenglContext::SetLeftCorner(float x, float y, float z) { GuiElement::Move(x - Rect().left, y - Rect().top, z); }

void OpenglContext::update_context() {
    auto rect = this->Rect();
    glViewport(rect.left, rect.top, rect.width, rect.height);
}
}  // namespace gui