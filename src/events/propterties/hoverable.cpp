#include "events/propterties/hoverable.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <stdexcept>

#include "events/event.h"
#include "events/observer.h"
#include "utils/active.h"

namespace events {

bool Hoverable::ContainsMouse(sf::Event event) {
    return this->Contains(math::Vector2f(event.mouseMove.x, event.mouseMove.y));
}

void Hoverable::BindMouseIn(events::Observer &observer, const events::EVENT_FUNC &function) {
    auto event_function = [this, function](sf::Event event) {
        ReturnOnDisable(false);
        if (mouse_in_flag_ || !this->ContainsMouse(event))
            return false;

        mouse_in_flag_ = true;
        return function(event);
    };

    mouse_in_event_ = observer.Bind(sf::Event::MouseMoved, event_function, this->position_.z);
    if (!mouse_out_event_)
        BindMouseOut(observer, [](sf::Event) { return false; });
}
void Hoverable::BindMouseOut(events::Observer &observer, const events::EVENT_FUNC &function) {
    auto event_function = [this, function](sf::Event event) {
        ReturnOnDisable(false);
        if (!mouse_in_flag_ || this->ContainsMouse(event))
            return false;

        mouse_in_flag_ = false;
        return function(event);
    };
    mouse_out_event_ = observer.Bind(sf::Event::MouseMoved, event_function, this->position_.z);
    if (!mouse_in_event_)
        BindMouseIn(observer, [](sf::Event) { return false; });
}
}  // namespace events
