#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "base.h"
#include "gui/propteries/scaleable.h"

namespace gui {
class Text : virtual public GuiElement, virtual public mixins::DefaultScale {
   public:
	Text(utils::Vector2f position, utils::Vector2f max_size);

	virtual utils::Vector2f LeftCorner() const override;

    virtual void Resize(utils::Vector2f size) override;
    virtual void SetPosition(utils::Vector2f position) override;
	virtual void SetText(const sf::String& text);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Text& SfText();

   protected:
	sf::Font font_;
    sf::Text text_;
};
}  // namespace gui
