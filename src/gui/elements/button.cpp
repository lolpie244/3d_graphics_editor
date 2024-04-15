#include "gui/elements/button.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <stdexcept>

#include "events/event.h"
#include "gui/elements/base.h"
#include "gui/elements/sprite_element.h"
#include "gui/elements/text.h"
#include "utils/vector2.h"

namespace gui {

ButtonText::ButtonText(Button* button) : Text(button->Position(), button->Size()) {
    this->Resize(this->Size());
    this->SetParent(button);
}

void ButtonText::Resize(utils::Vector2f size) { Text::Resize(size * borders_size_); }
void ButtonText::SetPosition(utils::Vector2f position) { Text::SetPosition(position + this->Size() * offset_); }

void ButtonText::SetOffset(utils::Vector2f offset) {
    auto old_offset = offset_;
    offset_ = offset;
    this->SetPosition(position_ - this->Size() * old_offset);
}

void ButtonText::SetBordersSize(utils::Vector2f borders_size) {
    auto old_borders_size = borders_size_;
    borders_size_ = {1 - borders_size.x, 1 - borders_size.y};
	// std::cout << "123 " << old_borders_size << " | " << borders_size_ << '\n';
    this->Resize(this->Size() / old_borders_size);
}

Button::Button() : text_(this) {}

Button::Button(utils::Vector2f position, utils::Vector2f size) : SpriteGuiElement(position, size), text_(this) {}

gui::ButtonText& Button::Text() { return text_; }

void Button::BindPress(events::Observer& observer, const events::EVENT_FUNC& function) {
    auto event_function = [this, function](sf::Event event) {
        update_texture();
        return function(event);
    };

    mixins::Clickable::BindPress(observer, event_function);
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
