#include "gui/elements/base.h"

#include <SFML/System/Vector2.hpp>

namespace gui {

GuiElement::GuiElement() {}

void GuiElement::Resize(sf::Vector2f size) {
    this->size_ = size;
    sprite_.setOrigin(size / 2.0f);
}

int GuiElement::Id() const { return id_; }
int GuiElement::Depth() const { return depth_; }
sf::Vector2f GuiElement::Position() const { return position_; }
sf::Vector2f GuiElement::Size() const { return size_; }
sf::Vector2f GuiElement::LeftCorner() const { return position_ - size_ / 2.0f; }
}  // namespace gui
