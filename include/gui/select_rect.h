#pragma once

#include "gui/base.h"

namespace gui {
class SelectRect : public GuiElement {
   private:
    using GuiElement::Move;
    using GuiElement::Resize;
    using GuiElement::Scale;

   public:
    SelectRect() { SetPosition(0, 0, 1000); }

    virtual sf::Rect<float> Rect() const { return selection_rect_; }

    void SetRect(unsigned int x, unsigned int y, int width, int heigth) {
        selection_rect_ = sf::Rect<float>(std::min(x, x + width), std::min(y, y + heigth), abs(width), abs(heigth));
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (IsActive()) {
            sf::RectangleShape rectangle(selection_rect_.getSize());
            rectangle.setPosition(selection_rect_.getPosition());
            rectangle.setOutlineColor(sf::Color::Red);
            rectangle.setOutlineThickness(1);
            rectangle.setFillColor(sf::Color::Transparent);

            target.draw(rectangle, states);
        }
    }

   protected:
    sf::Rect<float> selection_rect_;
};
}  // namespace gui
