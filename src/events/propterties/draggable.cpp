#include "events/propterties/draggable.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "events/propterties/clickable.h"
#include "math/points_cast.h"
#include "utils/active.h"

namespace events {
void Draggable::BindPress(events::Observer& observer, const events::EVENT_FUNC& function, MouseButtons buttons) {
    Clickable::BindPress(
        observer,
        [this, function](sf::Event event) {
            last_position_ = glm::vec2(event.mouseButton.x, event.mouseButton.y);
            return function(event);
        },
        buttons);
}

void Draggable::BindDrag(events::Observer& observer, const EVENT_FUNC& function, MouseButtons buttons) {
    if (!press_event_)
        this->BindPress(
            observer, [this](sf::Event event) { return true; }, buttons);

    auto func = [this, function](sf::Event event) {
        ReturnOnDisable(false);

        if (!this->IsPressed())
            return false;

        glm::vec2 new_position(event.mouseMove.x, event.mouseMove.y);
        auto move = new_position - last_position_;
        last_position_ = new_position;
        return function(event, move);
    };
    move_event_ = observer.Bind(sf::Event::MouseMoved, func, this->GetPosition().z);
}

void Draggable3D::BindDrag(events::Observer& observer, const EVENT_FUNC& function, MouseButtons buttons) {
    auto func = [this, function](sf::Event event, glm::vec2 moved) {
        glm::vec3 move = math::to_world_coords(stage::StageManager::Instance().windowSize() / 2.0f + moved);
        return function(event, move);
    };

    Draggable::BindDrag(observer, func, buttons);
}

void Draggable3D::BindPress(events::Observer& observer, const events::EVENT_FUNC& function, MouseButtons buttons) {
    Clickable3D::BindPress(
        observer,
        [this, function](sf::Event event) {
            last_position_ = glm::vec2(event.mouseButton.x, event.mouseButton.y);
            return function(event);
        },
        buttons);
}
}  // namespace events
