#include "gui/propteries/draggable.h"
#include <SFML/Window/Event.hpp>
#include "utils/vector2.h"


namespace gui::mixins {
	void Draggable::BindDrag(events::Observer &observer, const EVENT_FUNC &function) {
		this->BindPress(observer, [this](sf::Event event) {
			last_position_ = utils::Vector2f(event.mouseButton.x, event.mouseButton.y);
			return true;
		});

		auto func = [this, function](sf::Event event) {
			utils::Vector2f new_position(event.mouseMove.x, event.mouseMove.y);
			auto move = new_position - last_position_;
			last_position_ = new_position;
			return function(event, move);
		};
		move_event_ = observer.Bind(sf::Event::MouseMoved, func);
	}
}
