#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <list>
#include <memory>
#include <set>
#include <unordered_set>

#include "event.h"

namespace events {

class Observer {
   public:
    std::unique_ptr<mEventType> Bind(sf::Event::EventType event, const EVENT_FUNC& func, int depth = 0);
    std::unique_ptr<mEventType> KeyBind(const std::unordered_set<sf::Keyboard::Key>& keys, const EVENT_FUNC& func,
                                   int depth = 0);
    bool Notify(sf::Event& event);

   protected:
    void Unbind(mEventType* event);

   private:
    struct cmp {
        bool operator()(const mEventType* a, const mEventType* b) const {
            if (a->depth_ != b->depth_)
                return a->depth_ > b->depth_;
            return a->id_ < b->id_;
        }
    };
    std::set<mEventType*, cmp> events_[sf::Event::Count];
    friend mEventType;
};
}  // namespace events
