#pragma once

#include <SFML/Window/Event.hpp>
#include <list>
#include <memory>
#include <set>
#include <unordered_map>

#include "event.h"

namespace events {

class Observer {
   public:
    std::unique_ptr<Event> Bind(sf::Event::EventType event, const EVENT_FUNC& func, int depth = 0);
    bool Notify(sf::Event& event);

   protected:
    void Unbind(Event* event);

   private:
    struct cmp {
        bool operator()(const Event* a, const Event* b) const {
            if (a->depth_ != b->depth_)
                return a->depth_ > b->depth_;
            return a->id_ < b->id_;
        }
    };
    std::set<Event*, cmp> events_[sf::Event::Count];
    friend Event;
};
}  // namespace events
