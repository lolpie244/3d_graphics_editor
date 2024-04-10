#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "gui/elements/sprite_element.h"
#include "gui/elements/text.h"
#include "gui/propteries/clickable.h"
#include "gui/propteries/scaleable.h"

namespace gui {

class Button;

class ButtonText : virtual public Text {
   public:
    ButtonText(Button* button, utils::Vector2f border_size);
    virtual void Resize(utils::Vector2f size) override;

   private:
    utils::Vector2f borders_size_;
};

class Button : virtual public SpriteGuiElement, virtual public mixins::Clickable, virtual public mixins::Scaleable {
   public:
    Button(utils::Vector2f border_size = {0.1, 0.1});
    Button(utils::Vector2f position, utils::Vector2f size, utils::Vector2f border_size = {0.1, 0.1});

    virtual ~Button() = default;

    gui::ButtonText& Text();

    void BindPress(events::Observer& observer, const events::EVENT_FUNC& function) override;
    void OnRelease() override;

   private:
    void update_texture();

   protected:
    ButtonText text_;
    sf::Texture pressed_texture_;
};
}  // namespace gui
