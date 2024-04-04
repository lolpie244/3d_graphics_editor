#include "gui/elements/button.h"

#include <SFML/Window/Event.hpp>

#include "events/event.h"

namespace gui {

void Button::BindPress(events::Observer& observer, const events::EVENT_FUNC& function) {
    auto event_function = [this, &function](sf::Event event) {
        update_texture();
        return function(event);
    };

    mixins::Clickable::BindPress(observer, event_function);
}

void Button::OnRelease() { update_texture(); }

void Button::update_texture() {
	if (!this->IsPressed()) {
		this->sprite_.setTexture(texture_);
	} else {
		this->sprite_.setTexture(pressed_texture_);
	}
}

}  // namespace gui
