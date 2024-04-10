#include "gui/propteries/hoverable.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <stdexcept>

#include "events/event.h"
#include "events/observer.h"

namespace gui::mixins {

bool Hoverable::Contains(utils::Vector2f point) {
    if (check_transparecy_) {
        throw std::runtime_error("NOT IMPLEMENTED");
    }

    utils::Vector2f left_corner = this->LeftCorner();
    utils::Vector2f rigth_corner = this->LeftCorner() + this->Size();

    return left_corner.x <= point.x && point.x <= rigth_corner.x && left_corner.y <= point.y &&
           point.y <= rigth_corner.y;
}

bool Hoverable::ContainsMouse(sf::Event event) {
    return this->Contains(utils::Vector2f(event.mouseMove.x, event.mouseMove.y));
}

void Hoverable::BindMouseIn(events::Observer &observer, const events::EVENT_FUNC &function) {
    auto event_function = [this, function](sf::Event event) {
        if (!is_active_ || mouse_in_flag_ || !this->ContainsMouse(event))
            return false;

        mouse_in_flag_ = true;
        return function(event);
    };

    mouse_in_event_ = observer.Bind(sf::Event::MouseMoved, event_function, Depth());
}
void Hoverable::BindMouseOut(events::Observer &observer, const events::EVENT_FUNC &function) {
    auto event_function = [this, function](sf::Event event) {
        if (!is_active_ || !mouse_in_flag_ || this->ContainsMouse(event))
            return false;

        mouse_in_flag_ = false;
        return function(event);
    };
    mouse_out_event_ = observer.Bind(sf::Event::MouseMoved, event_function, Depth());
}
}  // namespace gui::mixins
