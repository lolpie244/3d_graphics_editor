#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>
#include <memory>

#include "events/observer.h"
#include "math/transform.h"

#include "utils/active.h"
#include "utils/uuid.h"

namespace events {
class Hoverable : virtual public math::Transform, virtual public Activatable {
   public:
    virtual ~Hoverable() = default;

    void BindMouseIn(events::Observer &observer, const events::EVENT_FUNC &function);
    void BindMouseOut(events::Observer &observer, const events::EVENT_FUNC &function);
    void BindMouseMove(events::Observer &observer, const events::EVENT_FUNC &function);

    virtual bool Contains(glm::vec2 point) = 0;
    bool ContainsMouse(sf::Event event);

   protected:
    events::Event mouse_in_event_;
    events::Event mouse_out_event_;
    events::Event mouse_move_event_;
    bool mouse_in_flag_ = false;
    bool check_transparecy_ = false;
};
class Hoverable3D : virtual public UUID, virtual public Hoverable {
   public:
    virtual bool Contains(glm::vec2 point);
};
}  // namespace events
