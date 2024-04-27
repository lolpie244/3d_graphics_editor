#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

#include "events/observer.h"
#include "gui/buttons_list.h"
#include "math/vector2.h"

namespace gui {
ButtonFromList::ButtonFromList(const sf::String& text) { this->Text().SetText(text); }

void ButtonFromList::AddButtonList(events::Observer& observer, ButtonsList* button_list) {
    button_list->Disable();
    button_list->BindMouseOut(observer, [button_list, this](sf::Event event) {
        button_list->Disable();
        return false;
    });

    this->BindMouseIn(observer, [button_list, this](sf::Event event) {
        button_list->Enable();
        auto coef = dynamic_cast<ButtonsList*>(this->parent)->orientation_ == ListOrientation::Vertical
                        ? math::Vector2f{0, this->Size().y}
                        : math::Vector2f{this->Size().x, 0};

        button_list->Resize(this->Size());
        button_list->SetPosition(this->Position() + glm::vec3(coef.x, coef.y, 0));
        return false;
    });

    this->BindMouseOut(observer, [button_list](sf::Event event) {
        if (!button_list->ContainsMouse(event))
            button_list->Disable();
        return false;
    });
}

ButtonsList::ButtonsList(ListOrientation orientation) : orientation_(orientation) {}

ButtonsList::ButtonsList(glm::vec3 position, math::Vector2f size, ListOrientation orientation)
    : orientation_(orientation) {
    this->SetPosition(position);
    this->Resize(size);
}

math::Vector2f ButtonsList::PositionCoef() const {
    return orientation_ == ListOrientation::Vertical ? math::Vector2f{0, this->Size().y}
                                                     : math::Vector2f{this->Size().x, 0};
}

sf::Rect<float> ButtonsList::Rect() const {
    if (buttons_.empty())
        return {0, 0, 0, 0};

    return {this->buttons_[0]->Rect().getPosition(),
            this->Size() + (float)(this->buttons_.size() - 1) * PositionCoef()};
}

void ButtonsList::Move(float x, float y, float z) {
    auto coef = PositionCoef();

    for (float i = 0; i < buttons_.size(); i++) {
        auto new_pos = math::Vector2f{x, y} + i * coef;
        buttons_[i]->SetPosition(new_pos.x, new_pos.y, z);
    }
}

void ButtonsList::AddButton(ButtonType button) {
    auto button_move_to = (float)buttons_.size() * PositionCoef();

    button->SetParent(this);
    button->Resize(this->Size());
    button->Move(button_move_to.x, button_move_to.y);
    button->Text().SfText().setFillColor(font_color_);
    button->SetPressedTexture(pressed_texture_);
    button->SetReleasedTexture(pressed_texture_);
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
    this->SetPosition(this->Position());
}

}  // namespace gui
