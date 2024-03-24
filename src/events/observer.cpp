#include "events/observer.h"
#include "events/event.h"

#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <memory>

namespace events {
std::unique_ptr<Event> Observer::Bind(sf::Event::EventType type, const EVENT_FUNC& func, int depth) {
	auto event = std::make_unique<Event>(*this, type, func, depth);
    events_[type].insert(event.get());

    return std::move(event);
}

bool Observer::Notify(sf::Event& event_data) {
    for (auto event : events_[event_data.type]) {
        if (event->function_(event_data))
            return true;
    }
    return false;
}

}  // namespace events
