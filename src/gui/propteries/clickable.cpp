#include "gui/propteries/clickable.h"

#include <SFML/Window/Event.hpp>

#include "events/event.h"
#include "events/observer.h"

namespace gui::mixins {
void Clickable::BindPress(events::Observer& observer, const events::EVENT_FUNC& function) {
    auto event_function = [this, &function](sf::Event event) {
        if (!this->ContainsMouse(event))
            return false;

        pressed_ = true;
        return function(event);
    };

    press_event_ = observer.Bind(sf::Event::MouseButtonPressed, event_function);
}

void Clickable::BindRelease(events::Observer& observer, const events::EVENT_FUNC& function) {
    auto event_function = [this, &function](sf::Event event) {
        if (!pressed_)
            return false;
        pressed_ = false;
        OnRelease();

        if (!this->ContainsMouse(event))
            return false;
        return function(event);
    };

    if (!press_event_)
        BindPress(observer, [](sf::Event) { return true; });

    release_event_ = observer.Bind(sf::Event::MouseButtonReleased, event_function);
}
}  // namespace gui::mixins
