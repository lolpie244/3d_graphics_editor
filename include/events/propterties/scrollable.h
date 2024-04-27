#pragma once

#include "events/propterties/hoverable.h"

namespace events {
class Scrollable : virtual public Hoverable {
   public:
    virtual ~Scrollable() = default;
    bool ContainsMouse(sf::Event event);

    virtual void BindScroll(events::Observer& observer, const events::EVENT_FUNC& function);

 protected:
	events::Event scroll_event_;
};
}  // namespace events
