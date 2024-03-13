#pragma once

#include <SFML/Window/Event.hpp>
#include <functional>
#include <list>
#include <unordered_map>

namespace events
{

template <typename EVENT_FUNC = std::function<bool()>> class Observer
{

public:
	int bind(int event, const EVENT_FUNC &func);
	bool notify(sf::Event& event);

private:
	std::unordered_map<sf::Event, std::list<EVENT_FUNC>> events_;
};
} // namespace events
