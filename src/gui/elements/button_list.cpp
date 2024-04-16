#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

#include "events/observer.h"
#include "gui/elements/buttons_list.h"
#include "utils/vector2.h"

namespace gui {
ButtonFromList::ButtonFromList(const sf::String& text) { this->Text().SetText(text); }

void ButtonFromList::AddButtonList(events::Observer& observer, ButtonsList* button_list, ListOrientation orientation) {
    button_list->Disable();
    button_list->BindMouseOut(observer, [button_list, this](sf::Event event) {
        button_list->Disable();
        return false;
    });

    this->BindMouseIn(observer, [button_list, orientation, this](sf::Event event) {
        button_list->Enable();
        auto coef = orientation == ListOrientation::Vertical ? utils::Vector2f{0, this->Size().y}
                                                             : utils::Vector2f{this->Size().x, 0};

        button_list->Resize(this->Size());
        button_list->SetPosition(this->Position() + coef);
        return false;
    });

    this->BindMouseOut(observer, [button_list](sf::Event event) {
        if (!button_list->ContainsMouse(event))
            button_list->Disable();
        return false;
    });
}

ButtonsList::ButtonsList(ListOrientation orientation) : orientation_(orientation) {}

ButtonsList::ButtonsList(utils::Vector2f position, utils::Vector2f size, ListOrientation orientation)
    : orientation_(orientation) {
    this->SetPosition(position);
    this->Resize(size);
}

utils::Vector2f ButtonsList::PositionCoef() const {
    return orientation_ == ListOrientation::Vertical ? utils::Vector2f{0, this->Size().y}
                                                     : utils::Vector2f{this->Size().x, 0};
}

sf::Rect<float> ButtonsList::Rect() const {
    if (buttons_.empty())
        return {0, 0, 0, 0};

    return {this->buttons_[0]->Rect().getPosition(),
            this->Size() + (float)(this->buttons_.size() - 1) * PositionCoef()};
}

void ButtonsList::SetPosition(utils::Vector2f position) {
    position_ = position;
    auto coef = PositionCoef();

    for (float i = 0; i < buttons_.size(); i++) { buttons_[i]->SetPosition(position + i * coef); }
}

void ButtonsList::AddButton(ButtonType button) {
    button->SetParent(this);
    button->Resize(this->Size());
    button->SetPosition(this->Position() + (float)buttons_.size() * PositionCoef());
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
