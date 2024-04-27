#include "gui/sprite_element.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "data/texture.h"
#include "utils/active.h"
#include "utils/settings.h"

namespace gui {
SpriteGuiElement::SpriteGuiElement(glm::vec3 position, math::Vector2f size) {
    this->SetPosition(position.x, position.y, position.z);
    this->Resize(size);
}

sf::Rect<float> SpriteGuiElement::Rect() const { return this->sprite_.getGlobalBounds(); }

void SpriteGuiElement::Resize(math::Vector2<float> size) {
    GuiElement::Resize(size);

    if (svg_texture_ == nullptr)
        return;

    auto real_size = sprite_.getLocalBounds().getSize();
    auto scale = size / real_size;

    if (std::max(scale.x, scale.y) >= settings::SVG_RESIZE_COEF ||
        std::min(scale.x, scale.y) <= 1 / settings::SVG_RESIZE_COEF) {
        this->SetTexture(svg_texture_);
        return;
    }

    sprite_.setScale(scale);
}

void SpriteGuiElement::Move(float x, float y, float z) {
    this->sprite_.setPosition(this->GetPosition().x + x, this->GetPosition().y + y);
    GuiElement::Move(x, y, z);
}

void SpriteGuiElement::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (IsActive()) {
        target.draw(this->sprite_, states);
        GuiElement::draw(target, states);
    }
}

void SpriteGuiElement::SetTexture(data::Texture new_texture) {
    svg_texture_ = new_texture;

    this->texture_ = svg_texture_->getTexture(this->size_);
    sprite_.setTexture(texture_, true);
    sprite_.setScale(1, 1);
    auto real_size = sprite_.getLocalBounds().getSize();
    sprite_.setOrigin(real_size.x / 2.0f, real_size.y / 2.0f);
}
}  // namespace gui
