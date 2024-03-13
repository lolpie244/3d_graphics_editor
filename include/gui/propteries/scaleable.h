#pragma once

#include "events/observer.h"
#include "gui/elements/base.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Window.hpp>

namespace gui::mixins
{
class Scaleable : virtual public gui::BaseGuiElement
{
public:
	virtual ~Scaleable() {}
	void bind_scale(events::Observer<bool(sf::Event)> &observer);

protected:
	virtual void scale(sf::RenderTarget &render) = 0;
};
} // namespace gui::mixins
