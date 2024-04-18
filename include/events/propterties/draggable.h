#pragma once

#include <SFML/System/Vector2.hpp>

#include "events/event.h"
#include "events/observer.h"
#include "clickable.h"

namespace events {

class Draggable : virtual public Clickable {
   public:
    typedef std::function<bool(sf::Event, math::Vector2f)> EVENT_FUNC;

   public:
    virtual ~Draggable() = default;

    void BindDrag(events::Observer &observer, const EVENT_FUNC &function);

   private:
	events::Event move_event_;

    math::Vector2f last_position_;
};
}  // namespace events
