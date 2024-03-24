#pragma once

#include "events/observer.h"
#include "events/event.h"
#include "gui/elements/base.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Window.hpp>

namespace gui::mixins
{
class Scaleable : virtual public gui::GuiElement
{
public:
	virtual ~Scaleable() {}
	void BindScale(events::Observer &observer);

protected:
	std::unique_ptr<events::Event> event;
	virtual void scale() = 0;


	// virtual void scale(sf::RenderTarget &render) = 0;
};
} // namespace gui::mixins
