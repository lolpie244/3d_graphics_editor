#include "gui/base.h"

#include <SFML/System/Vector2.hpp>

#include "math/transform.h"
#include "math/vector2.h"
#include "utils/active.h"

namespace gui {

std::atomic<int> GuiElement::max_object_id{0};

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

void GuiElement::Scale(float x, float y, float z) { Resize(size_ * math::Vector2f{x, y}); }

void GuiElement::Resize(math::Vector2f size) {
    size_ = size;
    RunForChilds(&GuiElement::Resize, this, size);
}

void GuiElement::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (IsActive())
        RunForChilds(&GuiElement::draw, this, target, states);
}

bool GuiElement::Contains(math::Vector2f point) { return this->Rect().contains(point); }

int GuiElement::Id() const { return id_; }
math::Vector2f GuiElement::Size() const { return size_; }
}  // namespace gui
