#pragma once

#include "events/observer.h"
#include "gui/propteries/hoverable.h"

namespace gui::mixins {
class Clickable : virtual public Hoverable {
   public:
    virtual ~Clickable() = default;

	bool ContainsMouse(sf::Event event);

    virtual void BindPress(events::Observer& observer, const events::EVENT_FUNC& function);
    virtual void BindRelease(events::Observer& observer, const events::EVENT_FUNC& function);

	virtual void OnRelease(){};

	bool IsPressed() const { return pressed_; }

   protected:
    std::unique_ptr<events::Event> press_event_;
    std::unique_ptr<events::Event> release_event_;

    bool pressed_ = false;
};
}  // namespace gui::mixins
