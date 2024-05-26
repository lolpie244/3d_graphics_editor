#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

#include "events/observer.h"
#include "gui/base.h"
#include "gui/buttons_list.h"

namespace gui {
ButtonFromList::ButtonFromList(const sf::String& text) { this->Text().SetText(text); }

void ButtonFromList::Enable() {
	GuiElement::Enable();
	if (button_list_)
		button_list_->Disable();
}

void ButtonFromList::AddButtonList(events::Observer& observer, std::shared_ptr<ButtonsList> button_list) {
    this->button_list_ = button_list;
    this->button_list_->SetParent(this);
    button_list->Disable();
    button_list->BindMouseOut(observer, [button_list, this](sf::Event event) {
        for (auto& button : button_list->buttons_) {
            if (button->button_list_ && button->button_list_->ContainsMouse(event))
                return false;
        }
		ButtonsList* parent = button_list_.get();
		while (parent != nullptr) {
			if (!parent->parent || parent->ContainsMouse(event))
				break;
			parent->Disable();
			auto button = dynamic_cast<ButtonFromList*>(parent->parent);
			if (!button)
				break;
			parent = dynamic_cast<ButtonsList*>(button->parent);
		}
        return false;
    });

    this->BindMouseIn(observer, [button_list, this](sf::Event event) {
        button_list->Enable();
        auto coef = dynamic_cast<ButtonsList*>(this->parent)->orientation_ == ListOrientation::Horizontal
                        ? glm::vec2{0, this->Size().y}
                        : glm::vec2{this->Size().x, 0};

        button_list->Resize(this->Size());
        button_list->SetPosition(this->GetPosition() + glm::vec3(coef.x, coef.y, 0));
        return false;
    });

    this->BindMouseOut(observer, [button_list](sf::Event event) {
        if (!button_list->ContainsMouse(event))
            button_list->Disable();
        return false;
    });
}

ButtonsList::ButtonsList(float space, ListOrientation orientation) : space_(space), orientation_(orientation) {}

ButtonsList::ButtonsList(glm::vec3 position, glm::vec2 size, float space, ListOrientation orientation)
    : space_(space), orientation_(orientation) {
    this->SetPosition(position);
    this->Resize(size);
}

glm::vec2 ButtonsList::PositionCoef() const {
    return orientation_ == ListOrientation::Vertical ? glm::vec2{0, this->Size().y + space_}
                                                     : glm::vec2{this->Size().x + space_, 0};
}

sf::Rect<float> ButtonsList::Rect() const {
    if (buttons_.empty())
        return {0, 0, 0, 0};
    auto size = this->Size() + (float)(this->buttons_.size() - 1) * PositionCoef();
    return {this->buttons_[0]->Rect().getPosition(), sf::Vector2f(size.x, size.y)};
}

void ButtonsList::Move(float x, float y, float z) {
    GuiElement::Move(x, y, z);

    auto coef = PositionCoef();

    for (float i = 0; i < buttons_.size(); i++) {
        auto new_pos = glm::vec2{x, y} + i * coef;
        buttons_[i]->SetPosition(new_pos.x, new_pos.y, z);
    }
}

void ButtonsList::AddButton(ButtonType button) {
    auto button_move_to = glm::vec2(position_) + (float)buttons_.size() * PositionCoef();

    button->SetParent(this);
    button->Resize(this->Size());
    button->Move(button_move_to.x, button_move_to.y);
    button->Text().SfText().setFillColor(font_color_);
    button->SetPressedTexture(pressed_texture_);
    button->SetReleasedTexture(released_texture_);
    button->is_active_ = this->is_active_;

    buttons_.push_back(button);
}

void ButtonsList::AddButtons(const std::initializer_list<ButtonType>& buttons) {
    for (auto& obj : buttons) AddButton(obj);
}

void ButtonsList::SetFontColor(sf::Color color) {
    font_color_ = color;
    for (auto& button : buttons_) { button->Text().SfText().setFillColor(color); }
}

void ButtonsList::SetReleasedTexture(Button::TextureInfo texture) {
    released_texture_ = texture;
    for (auto& button : buttons_) { button->SetReleasedTexture(released_texture_); }
}

void ButtonsList::SetPressedTexture(Button::TextureInfo texture) {
    pressed_texture_ = texture;
    for (auto& button : buttons_) { button->SetPressedTexture(pressed_texture_); }
}

void ButtonsList::SetOrientation(ListOrientation orientation) {
    orientation_ = orientation;
    this->SetPosition(this->GetPosition());
}

}  // namespace gui
