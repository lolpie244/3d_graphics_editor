#include "events/event.h"

#include "events/observer.h"

namespace events {

Event::Event(Observer& observer, sf::Event::EventType type, EVENT_FUNC func, int depth)
    : observer_(observer), type_(type), function_(func), depth_(depth) {}

Event::~Event() { observer_.Unbind(this); }

}  // namespace events
