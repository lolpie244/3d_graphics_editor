#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "base.h"
#include "utils/texture.h"

namespace gui {
class SpriteGuiElement : virtual public GuiElement {
   public:
    SpriteGuiElement(utils::Vector2f position, utils::Vector2f size);
    SpriteGuiElement() = default;
    virtual ~SpriteGuiElement() = default;

	sf::Rect<float> Rect() const override;

    virtual void Resize(utils::Vector2f size) override;
    virtual void SetPosition(utils::Vector2f position) override;
    virtual void SetTexture(std::shared_ptr<utils::Texture> new_texture);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   protected:
    sf::Sprite sprite_;
	sf::Texture texture_;
	std::shared_ptr<utils::Texture> svg_texture_;
};
}  // namespace gui
