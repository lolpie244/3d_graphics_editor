#include "gui/elements/base.h"

#include <SFML/System/Vector2.hpp>

namespace gui {

std::atomic<int> GuiElement::max_object_id{0};

void GuiElement::Enable() {
    is_active_ = true;
    RunForChilds(&GuiElement::Enable, this);
}
void GuiElement::Disable() {
    is_active_ = false;
    RunForChilds(&GuiElement::Disable, this);
}

void GuiElement::SetPosition(utils::Vector2f position) { RunForChilds(&GuiElement::SetPosition, this, position); }

void GuiElement::Resize(utils::Vector2f size) { RunForChilds(&GuiElement::Resize, this, size); }

void GuiElement::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    RunForChilds(&GuiElement::draw, this, target, states);
}

int GuiElement::Id() const { return id_; }
int GuiElement::Depth() const { return depth_; }
utils::Vector2f GuiElement::Position() const { return position_; }
utils::Vector2f GuiElement::Size() const { return size_; }

}  // namespace gui
