#pragma once

#include "events/observer.h"
#include "hoverable.h"

namespace events {
class Clickable : virtual public Hoverable {
   public:
    virtual ~Clickable() = default;

	bool ContainsMouse(sf::Event event);

    virtual void BindPress(events::Observer& observer, const events::EVENT_FUNC& function);
    virtual void BindRelease(events::Observer& observer, const events::EVENT_FUNC& function);

	virtual void OnRelease(){};

	bool IsPressed() const { return pressed_; }

   protected:
	events::Event press_event_;
    events::Event release_event_;

    bool pressed_ = false;
};
}  // namespace events
