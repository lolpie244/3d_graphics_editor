#include "events/propterties/clickable.h"

#include <SFML/Window/Event.hpp>

#include "events/event.h"
#include "events/observer.h"
#include "utils/active.h"

namespace events {
void Clickable::BindPress(events::Observer& observer, const events::EVENT_FUNC& function) {
    auto event_function = [this, function](sf::Event event) {
        ReturnOnDisable(false);
        if (!this->ContainsMouse(event))
            return false;

        pressed_ = true;
        return function(event);
    };

    press_event_ = observer.Bind(sf::Event::MouseButtonPressed, event_function, this->Position().z);

    if (!release_event_)
        BindRelease(observer, [](sf::Event) { return false; });
}

bool Clickable::ContainsMouse(sf::Event event) {
    return this->Contains(math::Vector2f(event.mouseButton.x, event.mouseButton.y));
}

void Clickable::BindRelease(events::Observer& observer, const events::EVENT_FUNC& function) {
    auto event_function = [this, function](sf::Event event) {
        if (!pressed_)
            return false;
        pressed_ = false;
        OnRelease();

        ReturnOnDisable(false);

        if (!this->ContainsMouse(event))
            return false;
        return function(event);
    };

    release_event_ = observer.Bind(sf::Event::MouseButtonReleased, event_function, this->Position().z);

    if (!press_event_)
        BindPress(observer, [](sf::Event) { return true; });
}
}  // namespace events
