#pragma once

#include <SFML/System/Vector2.hpp>

#include "events/observer.h"
#include "gui/elements/base.h"
#include "gui/propteries/clickable.h"

namespace gui::mixins {
class Draggable : virtual public Clickable {
   public:
    virtual ~Draggable();

    void bind_drag(events::Observer &observer, const events::EVENT_FUNC &function);

   protected:
    std::function<bool(sf::Event, sf::Vector2<float> moved_to)> drag_event_;

   private:
    sf::Vector2<float> last_position_;
};
}  // namespace gui::mixins
