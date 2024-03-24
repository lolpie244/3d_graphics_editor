#include "gui/propteries/scaleable.h"

#include <SFML/Window/Event.hpp>
#include <memory>

#include "events/observer.h"

namespace gui::mixins {
void Scaleable::BindScale(events::Observer &observer) {
    auto event_function = [this](sf::Event event) {
        this->scale();
        return false;
    };

	event = observer.Bind(sf::Event::Resized, event_function);
}
}  // namespace gui::mixins
