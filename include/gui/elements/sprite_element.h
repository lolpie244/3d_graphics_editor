#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "base.h"
#include "utils/texture.h"

namespace gui {
class SpriteGuiElement : virtual public GuiElement {
   public:
    SpriteGuiElement(sf::Vector2f position, sf::Vector2f size);
    virtual ~SpriteGuiElement() = default;

    sf::Vector2f LeftCorner() const override;

    virtual void Resize(sf::Vector2f size) override;
    virtual void SetPosition(sf::Vector2f position) override;
    virtual void SetTexture(std::shared_ptr<utils::Texture> new_texture);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   protected:
    sf::Sprite sprite_;
	sf::Texture texture_;
	std::shared_ptr<utils::Texture> svg_texture_;
};
}  // namespace gui
