#include <SFML/Graphics/Color.hpp>
#include <memory>

#include "gui/elements/buttons_list.h"
#include "utils/vector2.h"

namespace gui {
ButtonFromList::ButtonFromList(const sf::String& text) { this->Text().SetText(text); }

void ButtonFromList::SetPosition(utils::Vector2f position) {
    Button::SetPosition(position + this->Size() * utils::Vector2f{0.0f, (float)id_});
}

ButtonsList::ButtonsList(utils::Vector2f position, utils::Vector2f size) {
    this->SetPosition(position);
    this->Resize(size);
}

utils::Vector2f ButtonsList::LeftCorner() const {
    if (!buttons_.empty())
        return buttons_.begin()->second->LeftCorner();
    return {0, 0};
}

void ButtonsList::AddButton(const std::string& name, std::unique_ptr<ButtonFromList> button) {
    button->id_ = buttons_.size();

    button->SetParent(this);
    button->Resize(this->Size());
    button->SetPosition(this->Position());
    button->Text().SfText().setFillColor(font_color_);

    if (button_texture_)
        button->SetTexture(button_texture_);

    buttons_[name] = std::move(button);
}

void ButtonsList::AddButton(const std::string& name, const sf::String& text) {
	AddButton(name, std::make_unique<ButtonFromList>(text));
}

void ButtonsList::SetFontColor(sf::Color color) {
    font_color_ = color;
    for (auto& [_, button] : buttons_) { button->Text().SfText().setFillColor(color); }
}

void ButtonsList::SetButtonTexture(std::shared_ptr<utils::Texture> texture) {
    button_texture_ = texture;
    for (auto& [_, button] : buttons_) { button->SetTexture(button_texture_); }
}

ButtonFromList* ButtonsList::Button(const std::string& name) {
	return buttons_[name].get();
}

}  // namespace gui
