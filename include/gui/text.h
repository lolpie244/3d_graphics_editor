#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "base.h"
#include "events/propterties/scaleable.h"

namespace gui {
class Text : virtual public GuiElement, virtual public events::Scaleable {
   public:
	Text(glm::vec3 position, math::Vector2f max_size);
	Text() = default;

	sf::Rect<float> Rect() const override;

    virtual void Move(float x = 0, float y = 0, float z = 0) override;
    virtual void Resize(math::Vector2f size) override;
	virtual void SetText(const sf::String& text);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Text& SfText();

   protected:
	sf::Font font_;
    sf::Text text_;
};
}  // namespace gui
