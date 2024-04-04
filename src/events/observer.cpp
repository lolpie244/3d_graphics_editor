#include "events/observer.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <memory>
#include <unordered_set>

namespace events {
std::unique_ptr<Event> Observer::Bind(sf::Event::EventType type, const EVENT_FUNC& func, int depth) {
    auto event = std::make_unique<Event>(*this, type, func, depth);
    events_[type].insert(event.get());

    return std::move(event);
}

std::unique_ptr<Event> Observer::KeyBind(const std::unordered_set<sf::Keyboard::Key>& keys, const EVENT_FUNC& func,
                                         int depth) {
    auto press_func = [keys, func](sf::Event event) {
        if (!keys.contains(event.key.code))
            return false;

        for (auto& key : keys) {
            if (!sf::Keyboard::isKeyPressed(key))
                return false;
        }
        return func(event);
    };

    auto event = std::make_unique<Event>(*this, sf::Event::EventType::KeyPressed, press_func, depth);
    events_[event->type_].insert(event.get());

    return std::move(event);
}

bool Observer::Notify(sf::Event& event_data) {
    for (auto event : events_[event_data.type]) {
        if (event->function_(event_data))
            return true;
    }
    return false;
}

void Observer::Unbind(Event* event) { events_[event->type_].erase(event); }

}  // namespace events
