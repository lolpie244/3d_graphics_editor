#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "base.h"

namespace gui {
class Text : virtual public GuiElement {
   public:
	Text(sf::Vector2f position, sf::Vector2f max_size);

	virtual sf::Vector2f LeftCorner() const override;

    virtual void Resize(sf::Vector2f size) override;
    virtual void SetPosition(sf::Vector2f position) override;
	virtual void SetText(const sf::String& text);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Text& SfText();

   protected:
	sf::Font font_;
    sf::Text text_;
};
}  // namespace gui