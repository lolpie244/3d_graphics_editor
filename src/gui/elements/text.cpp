#include "gui/elements/text.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace gui {

Text::Text(sf::Vector2f max_size, sf::Vector2f position) {
    font_.loadFromFile("font.ttf");
    this->text_.setFont(font_);

    this->Resize(max_size);
    this->SetPosition(position);
}

sf::Vector2f Text::LeftCorner() const {
	return text_.getLocalBounds().getPosition();
}

void Text::SetText(std::string text) {
    text_.setString(text);
    this->Resize(this->size_);
}

void Text::SetFontSize(uint size) {
	text_.setCharacterSize(size);
	this->Resize(this->size_);
}

void Text::Resize(sf::Vector2f size) {
    auto real_size = text_.getLocalBounds().getSize();
    auto scale = std::min(std::min(real_size.x, size.x) / real_size.x, std::min(real_size.y, size.y) / real_size.y);
    text_.setScale(scale, scale);
    text_.setOrigin(0, real_size.y / 2.0f);
    size_ = size;
}

void Text::SetPosition(sf::Vector2f position) {
    this->text_.setPosition(position);
    position_ = position;
}

sf::Text& Text::SfText() { return text_; }

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    //   if (is_active_)
    // std::cout << "SOMEWHERE HERE\n";
    target.draw(this->text_, states);
}

}  // namespace gui
