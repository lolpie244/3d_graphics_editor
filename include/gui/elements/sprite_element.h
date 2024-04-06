#pragma once

#include "base.h"
namespace gui {
class SpriteGuiElement : virtual public GuiElement {
   public:
	SpriteGuiElement(sf::Vector2f position, sf::Vector2f size);
	virtual ~SpriteGuiElement() = default;

    virtual void Resize(sf::Vector2f size) override;
    virtual void SetPosition(sf::Vector2f position) override;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
   protected:
    sf::Sprite sprite_;
    sf::Texture texture_;
};
}
