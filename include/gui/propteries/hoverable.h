#pragma once
#include "events/observer.h"
#include "gui/elements/base.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>

namespace gui::mixins
{
class Hoverable : virtual public BaseGuiElement
{
public:
	virtual ~Hoverable();

	void bind_mouse_in(std::function<bool(sf::Event)> function,
	                   events::Observer<bool(sf::Event)> observer);
	void bind_mouse_out(std::function<bool(sf::Event)> function,
	                   events::Observer<bool(sf::Event)> observer);

protected:
	virtual bool is_mouse_in(sf::Vector2<float> mouse);
protected:
	std::function<bool(sf::Event)> mouse_in_event_;
	std::function<bool(sf::Event)> mouse_out_event_;
	bool mouse_in_flag_ = false;
	bool check_transparecy = false;
};
} // namespace gui::mixins
