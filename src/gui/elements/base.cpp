#include "gui/elements/base.h"

#include <SFML/System/Vector2.hpp>

namespace gui {

std::atomic<int> GuiElement::max_object_id{0};

void GuiElement::Enable() { is_active_ = true; }
void GuiElement::Disable() { is_active_ = false; }

int GuiElement::Id() const { return id_; }
int GuiElement::Depth() const { return depth_; }
sf::Vector2f GuiElement::Position() const { return position_; }
sf::Vector2f GuiElement::Size() const { return size_; }
sf::Vector2f GuiElement::LeftCorner() const { return position_ - size_ / 2.0f; }

}  // namespace gui
