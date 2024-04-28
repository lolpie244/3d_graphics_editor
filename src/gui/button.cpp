#include "gui/button.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <stdexcept>

#include "events/event.h"
#include "gui/sprite_element.h"
#include "gui/text.h"
#include "math/vector2.h"

namespace gui {

ButtonText::ButtonText(Button* button) : Text(button->GetPosition(), button->Size()) {
    this->Resize(this->Size());
    this->SetParent(button);
}

void ButtonText::Resize(math::Vector2f size) { Text::Resize(size * borders_size_); }
void ButtonText::SetPosition(float x, float y, float z) {
    auto new_pos = math::Vector2f{x, y} + this->Size() * offset_;
    Text::SetPosition(new_pos.x, new_pos.y, z);
}

void ButtonText::SetOffset(math::Vector2f offset) {
    auto old_offset = offset_;
    offset_ = offset;
    auto new_position = math::Vector2f(this->GetPosition()) - this->Size() * old_offset;

    this->SetPosition(new_position.x, new_position.y, this->GetPosition().z);
}

void ButtonText::SetBordersSize(math::Vector2f borders_size) {
    auto old_borders_size = borders_size_;
    borders_size_ = {1 - borders_size.x, 1 - borders_size.y};
    this->Resize(this->Size() / old_borders_size);
}

Button::Button() : text_(this) {}

Button::Button(glm::vec3 position, math::Vector2f size) : SpriteGuiElement(position, size), text_(this) {}

gui::ButtonText& Button::Text() { return text_; }

void Button::BindPress(events::Observer& observer, const events::EVENT_FUNC& function, MouseButtons buttons) {
    auto event_function = [this, function](sf::Event event) {
        update_texture();
        return function(event);
    };

    events::Clickable::BindPress(observer, event_function, buttons);
}

void Button::OnRelease() { update_texture(); }

void Button::SetPressedTexture(TextureInfo texture) {
    pressed_texture_ = texture;
    update_texture();
}

void Button::SetReleasedTexture(TextureInfo texture) {
    released_texture_ = texture;
    update_texture();
}

void Button::update_texture() {
    TextureInfo* current_texture_;

    if (released_texture_.texture && (!this->IsPressed() || pressed_texture_.texture == nullptr)) {
        current_texture_ = &released_texture_;
    } else {
        current_texture_ = &pressed_texture_;
    }
    if (current_texture_->texture == nullptr) {
        return;
    }

    this->SetTexture(current_texture_->texture);
    this->Text().SetOffset(current_texture_->offset);
    this->Text().SetBordersSize(current_texture_->borders_size);
}

}  // namespace gui
