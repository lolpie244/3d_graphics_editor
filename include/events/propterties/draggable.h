#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
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
    typedef std::function<bool(sf::Event, glm::vec2)> EVENT_FUNC;

   public:
    virtual ~Draggable() = default;

    virtual void BindDrag(events::Observer& observer, const EVENT_FUNC& function, MouseButtons buttons = all_buttons_);

   protected:
    virtual void on_start(sf::Event event);

   protected:
    glm::vec2 last_position_;
    glm::vec2 start_position_;

   private:
    bool is_pressed_ = false;
    events::Event press_event_;
    events::Event release_event_;
    events::Event move_event_;
};

class Draggable3D : virtual public Clickable3D, virtual public Draggable {
   public:
    typedef std::function<bool(sf::Event, glm::vec3)> EVENT_FUNC;

   public:
    virtual void BindDrag(events::Observer& observer, const EVENT_FUNC& function, MouseButtons buttons = all_buttons_);
    render::PickingTexture::Info StartPointInfo() { return start_point_info_; };

   protected:
    void on_start(sf::Event event) override;

 private:
	render::PickingTexture::Info start_point_info_;
};
}  // namespace events
