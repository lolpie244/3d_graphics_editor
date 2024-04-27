#include "events/propterties/draggable.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "events/propterties/clickable.h"
#include "math/points_cast.h"
#include "utils/active.h"

namespace events {
void Draggable::BindPress(events::Observer& observer, const events::EVENT_FUNC& function) {
    Clickable::BindPress(observer, [this, function](sf::Event event) {
        last_position_ = math::Vector2f(event.mouseButton.x, event.mouseButton.y);
        return function(event);
    });
}

void Draggable::BindDrag(events::Observer& observer, const EVENT_FUNC& function) {
    if (!press_event_)
        this->BindPress(observer, [this](sf::Event event) { return false; });

    auto func = [this, function](sf::Event event) {
        ReturnOnDisable(false);

        if (!this->IsPressed())
            return false;

        math::Vector2f new_position(event.mouseMove.x, event.mouseMove.y);
        auto move = new_position - last_position_;
        last_position_ = new_position;
        return function(event, move);
    };
    move_event_ = observer.Bind(sf::Event::MouseMoved, func, this->GetPosition().z);
}

void Draggable3D::BindDrag(events::Observer& observer, const EVENT_FUNC& function) {
    auto func = [this, function](sf::Event event, math::Vector2f moved) {
        glm::vec3 move = math::to_world_coords(stage::StageManager::Instance().windowSize() / 2.0f + moved);
        return function(event, move);
    };

    Draggable::BindDrag(observer, func);
}

void Draggable3D::BindPress(events::Observer& observer, const events::EVENT_FUNC& function) {
    Clickable3D::BindPress(observer, [this, function](sf::Event event) {
        last_position_ = math::Vector2f(event.mouseButton.x, event.mouseButton.y);
        return function(event);
    });
}
}  // namespace events
