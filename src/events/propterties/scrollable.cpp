#include "events/propterties/scrollable.h"

namespace events {

bool Scrollable::ContainsMouse(sf::Event event) {
    return this->Contains(glm::vec2(event.mouseWheelScroll.x, event.mouseWheelScroll.y));
}

void Scrollable::BindScroll(events::Observer& observer, const events::EVENT_FUNC& function) {
    auto event_function = [this, function](sf::Event event) {
        ReturnOnDisable(false);
        if (!this->ContainsMouse(event))
            return false;

        return function(event);
    };

    scroll_event_ = observer.Bind(sf::Event::MouseWheelScrolled, event_function);
}
}  // namespace events
