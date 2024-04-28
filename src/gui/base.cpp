#include "gui/base.h"

#include <SFML/System/Vector2.hpp>

#include "math/transform.h"
#include "utils/active.h"

namespace gui {

void GuiElement::Enable() {
    Activatable::Enable();
    RunForChilds(&GuiElement::Enable, this);
}
void GuiElement::Disable() {
    Activatable::Disable();
    RunForChilds(&GuiElement::Disable, this);
}

void GuiElement::Move(float x, float y, float z) {
    math::Transform::Move(x, y, z);
    RunForChilds(&GuiElement::Move, this, x, y, z);
}

void GuiElement::Scale(float x, float y, float z) { Resize(size_ * glm::vec2{x, y}); }

void GuiElement::Resize(glm::vec2 size) {
    size_ = size;
    RunForChilds(&GuiElement::Resize, this, size);
}

void GuiElement::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (IsActive())
        RunForChilds(&GuiElement::draw, this, target, states);
}

bool GuiElement::Contains(glm::vec2 point) { return this->Rect().contains(point.x, point.y); }

glm::vec2 GuiElement::Size() const { return size_; }
}  // namespace gui
