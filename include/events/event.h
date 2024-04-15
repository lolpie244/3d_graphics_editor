#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include <memory>

namespace events {
class Observer;
class mEventType;

typedef std::function<bool(sf::Event)> EVENT_FUNC;
typedef std::unique_ptr<mEventType> Event;

class mEventType {
   public:
    mEventType(Observer& observer, sf::Event::EventType type, EVENT_FUNC func, int depth = 0);

    virtual ~mEventType();

   protected:
    int depth_;
    int id_ = max_event_id++;
    EVENT_FUNC function_;
    Observer& observer_;
    sf::Event::EventType type_;

    static int max_event_id;

    friend Observer;
};

}  // namespace events
