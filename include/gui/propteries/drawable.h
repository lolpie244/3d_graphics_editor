#pragma once

#include <SFML/Graphics/Drawable.hpp>

#include "gui/elements/base.h"

namespace gui::mixins {
class SpriteDraw : virtual public gui::GuiElement, public sf::Drawable {
   public:
    virtual ~SpriteDraw() {}

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
}  // namespace gui::mixins
