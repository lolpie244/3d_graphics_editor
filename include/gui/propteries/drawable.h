#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include "gui/elements/base.h"

namespace gui::mixins {
class Drawable: virtual public gui::BaseGuiElement, public sf::Drawable {
public:
	virtual ~Drawable(){}
	virtual void draw() = 0;
};
}
