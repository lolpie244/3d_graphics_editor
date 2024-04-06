#include "gui/elements/sprite_element.h"

namespace gui {
SpriteGuiElement::SpriteGuiElement(sf::Vector2f position, sf::Vector2f size) {
	this->SetPosition(position);
	this->Resize(size);
}

void SpriteGuiElement::Resize(sf::Vector2f size) {
    this->size_ = size;
    sprite_.setOrigin(size / 2.0f);
}

void SpriteGuiElement::SetPosition(sf::Vector2f position) {
    this->sprite_.setPosition(position);
    position_ = position;
}

void SpriteGuiElement::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (is_active_)
        target.draw(this->sprite_, states);
}
}
