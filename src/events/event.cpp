#include "events/event.h"

#include "events/observer.h"

namespace events {

int mEventType::max_event_id = 0;

mEventType::mEventType(Observer& observer, sf::Event::EventType type, EVENT_FUNC func, int depth)
    : observer_(observer), type_(type), function_(func), depth_(depth) {}

mEventType::~mEventType() { observer_.Unbind(this); }
}  // namespace events
