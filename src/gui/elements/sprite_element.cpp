#include "gui/elements/sprite_element.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

#include "gui/elements/base.h"
#include "utils/settings.h"
#include "utils/texture.h"

namespace gui {
SpriteGuiElement::SpriteGuiElement(sf::Vector2f position, sf::Vector2f size) {
    this->SetPosition(position);
    this->Resize(size);
}

sf::Vector2f SpriteGuiElement::LeftCorner() const { return this->sprite_.getGlobalBounds().getPosition(); }

void SpriteGuiElement::Resize(sf::Vector2f size) {
    size_ = size;
    if (svg_texture_ == nullptr)
        return;

    auto real_size = sprite_.getLocalBounds();
    auto scale = sf::Vector2f(real_size.width / size.x, real_size.height / size.y);

    if (std::max(scale.x, scale.y) >= settings::SVG_RESIZE_COEF ||
        std::min(scale.x, scale.y) <= 1 / settings::SVG_RESIZE_COEF) {
        this->SetTexture(svg_texture_);
        return;
    }
    sprite_.scale(scale.x, scale.y);
    GuiElement::Resize(size);
}

void SpriteGuiElement::SetPosition(sf::Vector2f position) {
    this->sprite_.setPosition(position);
    position_ = position;
    GuiElement::SetPosition(position);
}

void SpriteGuiElement::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (is_active_) {
        target.draw(this->sprite_, states);
        GuiElement::draw(target, states);
    }
}

void SpriteGuiElement::SetTexture(std::shared_ptr<utils::Texture> new_texture) {
    svg_texture_ = new_texture;

    this->texture_ = svg_texture_->getTexture(this->size_);
    sprite_.setTexture(texture_, true);
    auto real_size = sprite_.getLocalBounds().getSize();
    sprite_.setOrigin(real_size.x / 2.0f, real_size.y / 2.0f);
}
}  // namespace gui