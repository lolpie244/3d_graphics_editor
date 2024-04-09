#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "gui/elements/sprite_element.h"
#include "gui/elements/text.h"
#include "gui/propteries/clickable.h"

namespace gui {

class Button;

class ButtonText : virtual public Text {
   public:
    ButtonText(Button* button, sf::Vector2f border_size);
    virtual void Resize(sf::Vector2f size) override;

   private:
    sf::Vector2f borders_size_;
};

class Button : virtual public SpriteGuiElement, virtual public mixins::Clickable {
   public:
    Button(sf::Vector2f position, sf::Vector2f size, sf::Vector2f border_size = sf::Vector2f(0.1, 0.1));
    virtual ~Button() = default;

    virtual void SetPosition(sf::Vector2f position) override;

    gui::ButtonText& Text();

    void BindPress(events::Observer& observer, const events::EVENT_FUNC& function) override;
    void OnRelease() override;

   private:
    void update_texture();

   private:
    ButtonText text_;
    sf::Texture pressed_texture_;
};
}  // namespace gui
