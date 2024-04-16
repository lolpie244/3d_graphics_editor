#include "gui/propteries/clickable.h"

#include <SFML/Window/Event.hpp>

#include "events/event.h"
#include "events/observer.h"

namespace gui::mixins {
void Clickable::BindPress(events::Observer& observer, const events::EVENT_FUNC& function) {
    auto event_function = [this, function](sf::Event event) {
        if (!is_active_ || !this->ContainsMouse(event))
            return false;

        pressed_ = true;
        return function(event);
    };

    press_event_ = observer.Bind(sf::Event::MouseButtonPressed, event_function);

    if (!release_event_)
        BindRelease(observer, [](sf::Event) { return false; });
}

bool Clickable::ContainsMouse(sf::Event event) {
    return this->Contains(utils::Vector2f(event.mouseButton.x, event.mouseButton.y));
}

void Clickable::BindRelease(events::Observer& observer, const events::EVENT_FUNC& function) {
    auto event_function = [this, function](sf::Event event) {
        if (!pressed_)
            return false;
        pressed_ = false;
        OnRelease();

        if (!is_active_ || !this->ContainsMouse(event))
            return false;
        return function(event);
    };

    release_event_ = observer.Bind(sf::Event::MouseButtonReleased, event_function);

    if (!press_event_)
        BindPress(observer, [](sf::Event) { return true; });
}
}  // namespace gui::mixins
