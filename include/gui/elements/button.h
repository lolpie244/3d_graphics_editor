#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "gui/elements/sprite_element.h"
#include "gui/elements/text.h"
#include "gui/propteries/clickable.h"

namespace gui {
class Button : virtual public SpriteGuiElement, virtual public mixins::Clickable {
   public:
    Button(sf::Vector2f position, sf::Vector2f size, sf::Vector2f border_size = sf::Vector2f(0.1, 0.1));
    virtual ~Button() = default;

    virtual void Resize(sf::Vector2f size) override;
    virtual void SetPosition(sf::Vector2f position) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    gui::Text& Text();

    void BindPress(events::Observer& observer, const events::EVENT_FUNC& function) override;
    void OnRelease() override;

   private:
    void update_texture();

   private:
	sf::Vector2f borders_size_;
    gui::Text text_;
    sf::Texture pressed_texture_;
};
}  // namespace gui
