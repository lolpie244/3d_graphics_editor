#include "gui/elements/text.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace gui {

Text::Text(utils::Vector2f position, utils::Vector2f max_size) {
    font_.loadFromFile("resources/other_font.ttf");
    this->text_.setFont(font_);

    this->Resize(max_size);
    this->SetPosition(position);
}

sf::Rect<float> Text::Rect() const { return text_.getGlobalBounds(); }

void Text::SetText(const sf::String& text) {
    text_.setString(text);
    this->Resize(this->size_);
}

void Text::Resize(utils::Vector2f size) {
	text_.setCharacterSize(size.y);

    auto real_size = text_.getLocalBounds().getSize();
    auto bounds = text_.getLocalBounds();
    text_.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

    auto scale = std::min(real_size.x, size.x) / real_size.x;
    text_.setScale(scale, scale);
    size_ = size;
}

void Text::SetPosition(utils::Vector2f position) {
    this->text_.setPosition(position);
    position_ = position;
}

sf::Text& Text::SfText() { return text_; }

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (is_active_)
        target.draw(this->text_, states);
}

}  // namespace gui
