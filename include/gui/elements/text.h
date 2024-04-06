#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "base.h"

namespace gui {
class Text : virtual public GuiElement {
   public:
	Text(sf::Vector2f max_size, sf::Vector2f position);

	virtual sf::Vector2f LeftCorner() const override;

    virtual void Resize(sf::Vector2f size) override;
    virtual void SetPosition(sf::Vector2f position) override;
	virtual void SetText(std::string text);
	virtual void SetFontSize(uint size);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Text& SfText();

   protected:
	sf::Font font_;
    sf::Text text_;
};
}  // namespace gui
