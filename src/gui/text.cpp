#include "gui/text.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <glm/ext/vector_float3.hpp>

#include "math/transform.h"
#include "utils/settings.h"

namespace gui {

Text::Text(glm::vec3 position, glm::vec2 max_size) {
    font_.loadFromFile(settings::FONT_FILE);
    this->text_.setFont(font_);
    this->text_.setFillColor(sf::Color::Black);

    this->Resize(max_size);
    this->SetPosition(position.x, position.y, position.z);
}

sf::Rect<float> Text::Rect() const { return text_.getGlobalBounds(); }

void Text::SetText(const sf::String& text) {
    text_.setString(text);
    this->Resize(this->size_);
}

void Text::Resize(glm::vec2 size) {
    text_.setCharacterSize(size.y);

    auto real_size = text_.getLocalBounds().getSize();
    auto bounds = text_.getLocalBounds();
    text_.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

    auto scale = std::min(real_size.x, size.x) / real_size.x;
    text_.setScale(scale, scale);
    size_ = size;
}

void Text::Move(float x, float y, float z) {
    this->text_.setPosition(this->GetPosition().x + x, this->GetPosition().y + y);
    GuiElement::Move(x, y, z);
}

sf::Text& Text::SfText() { return text_; }

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (IsActive())
        target.draw(this->text_, states);
}

}  // namespace gui
