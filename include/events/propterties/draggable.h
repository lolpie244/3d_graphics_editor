#pragma once

#include <SFML/System/Vector2.hpp>
#include <glm/ext/vector_float3.hpp>

#include "clickable.h"
#include "events/event.h"
#include "events/observer.h"
#include "events/propterties/hoverable.h"
#include "render/opengl/picking_texture.h"

namespace events {

class Draggable3D;

class Draggable : virtual public Clickable {
	friend Draggable3D;
   public:
    typedef std::function<bool(sf::Event, math::Vector2f)> EVENT_FUNC;

   public:
    virtual ~Draggable() = default;

    virtual void BindPress(events::Observer& observer, const events::EVENT_FUNC& function) override;
    virtual void BindDrag(events::Observer& observer, const EVENT_FUNC& function);

   private:
    math::Vector2f last_position_;
    events::Event move_event_;
};

class Draggable3D : virtual public Clickable3D, virtual public Draggable {
   public:
    typedef std::function<bool(sf::Event, glm::vec3)> EVENT_FUNC;

   public:
    virtual void BindPress(events::Observer& observer, const events::EVENT_FUNC& function);
    virtual void BindDrag(events::Observer& observer, const EVENT_FUNC& function);
};
}  // namespace events
