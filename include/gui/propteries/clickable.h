#pragma once

#include "events/observer.h"
#include "gui/elements/base.h"
#include "gui/propteries/hoverable.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>

namespace gui::mixins
{
class Clickable : virtual public Hoverable
{
public:
	virtual ~Clickable();

	void bind_press(std::function<bool(sf::Event)> function,
	                events::Observer<bool(sf::Event)> observer);
	void bind_release(std::function<bool(sf::Event)> function,
	                events::Observer<bool(sf::Event)> observer);
protected:
protected:
	std::function<bool(sf::Event)> press_event_;
	std::function<bool(sf::Event)> release_event_;
};
} // namespace gui::mixins
