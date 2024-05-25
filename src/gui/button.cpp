#include "gui/button.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <stdexcept>

#include "events/event.h"
#include "gui/base.h"
#include "gui/sprite_element.h"
#include "gui/text.h"

namespace gui {

ButtonText::ButtonText(Button* button) : Text(button->GetPosition(), button->Size()) {
    this->Resize(this->Size());
    this->SetParent(button);
}

void ButtonText::Resize(glm::vec2 size) { Text::Resize(size * borders_size_); }
void ButtonText::SetPosition(float x, float y, float z) {
    auto new_pos = glm::vec2{x, y} + this->Size() * offset_;
    Text::SetPosition(new_pos.x, new_pos.y, z);
}

void ButtonText::SetOffset(glm::vec2 offset) {
    auto old_offset = offset_;
    offset_ = offset;
    auto new_position = glm::vec2(this->GetPosition()) - this->Size() * old_offset;

    this->SetPosition(new_position.x, new_position.y, this->GetPosition().z);
}

void ButtonText::SetBordersSize(glm::vec2 borders_size) {
    auto old_borders_size = borders_size_;
    borders_size_ = {1 - borders_size.x, 1 - borders_size.y};
    this->Resize(this->Size() / old_borders_size);
}

Button::Button() : text_(this) {}

Button::Button(glm::vec3 position, glm::vec2 size) : SpriteGuiElement(position, size), text_(this) {}

gui::ButtonText& Button::Text() { return text_; }

void Button::BindPress(events::Observer& observer, const events::EVENT_FUNC& function, MouseButtons buttons) {
    auto event_function = [this, function](sf::Event event) {
        UpdateTexture();
        return function(event);
    };

    events::Clickable::BindPress(observer, event_function, buttons);
}

void Button::OnRelease(sf::Event event) { UpdateTexture(); }

void Button::SetPressedTexture(TextureInfo texture) {
    pressed_texture_ = texture;
    UpdateTexture();
}

void Button::SetReleasedTexture(TextureInfo texture) {
    released_texture_ = texture;
    UpdateTexture();
}

void Button::UpdateTexture() {
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

void Button::SetPosition(float x, float y, float z) {
	SpriteGuiElement::SetPosition(x, y, z);
	text_.SetPosition(x, y, z);
}


}  // namespace gui
