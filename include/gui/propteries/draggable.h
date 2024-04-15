#pragma once

#include <SFML/System/Vector2.hpp>

#include "events/event.h"
#include "events/observer.h"
#include "gui/elements/base.h"
#include "gui/propteries/clickable.h"

namespace gui::mixins {

class Draggable : virtual public Clickable {
   public:
    typedef std::function<bool(sf::Event, utils::Vector2f)> EVENT_FUNC;

   public:
    virtual ~Draggable() = default;

    void BindDrag(events::Observer &observer, const EVENT_FUNC &function);

   private:
	events::Event move_event_;

    utils::Vector2f last_position_;
};
}  // namespace gui::mixins
