#include "gui/elements/button.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

#include "events/event.h"
#include "gui/elements/sprite_element.h"
#include "gui/elements/text.h"

namespace gui {

ButtonText::ButtonText(Button* button, sf::Vector2f border_size)
    : Text(button->Position(), button->Size()), borders_size_(sf::Vector2f(1 - border_size.x, 1 - border_size.y)) {
    this->Resize(this->Size());
    this->SetParent(button);
}

void ButtonText::Resize(sf::Vector2f size) {
    Text::Resize(sf::Vector2f(size.x * borders_size_.x, size.y * borders_size_.y));
}

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Vector2f border_size)
    : SpriteGuiElement(position, size), text_(this, border_size) {}

gui::ButtonText& Button::Text() { return text_; }

void Button::SetPosition(sf::Vector2f position) {
    SpriteGuiElement::SetPosition(position);
    this->text_.SetPosition(position);
}

void Button::BindPress(events::Observer& observer, const events::EVENT_FUNC& function) {
    auto event_function = [this, function](sf::Event event) {
        update_texture();
        return function(event);
    };

    mixins::Clickable::BindPress(observer, event_function);
}

void Button::OnRelease() { update_texture(); }

void Button::update_texture() {
    if (!this->IsPressed()) {
        this->sprite_.setTexture(texture_);
    } else {
        // this->sprite_.setTexture(pressed_texture_);
    }
}

}  // namespace gui
