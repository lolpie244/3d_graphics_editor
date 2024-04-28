#include "events/propterties/draggable.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "events/propterties/clickable.h"
#include "math/points_cast.h"
#include "stage/stage_manager.h"
#include "utils/active.h"

namespace events {

void Draggable::on_start(sf::Event event) {
	last_position_ = glm::vec2(event.mouseButton.x, event.mouseButton.y);
	start_position_ = last_position_;
}

void Draggable::BindDrag(events::Observer& observer, const EVENT_FUNC& function, MouseButtons buttons) {
    auto press_func = [this, buttons](sf::Event event) {
        ReturnOnDisable(false);
        if (!this->ContainsMouse(event) || !buttons.contains(event.mouseButton.button))
            return false;
        is_pressed_ = true;
		on_start(event);
        return Clickable::press_event_ == nullptr;
    };

    auto release_func = [this](sf::Event event) {
        is_pressed_ = false;
		start_position_ = {-1, -1};
        return false;
    };

    auto move_func = [this, function](sf::Event event) {
        ReturnOnDisable(false);

        if (!is_pressed_)
            return false;
        glm::vec2 new_position(event.mouseMove.x, event.mouseMove.y);
        auto move = new_position - last_position_;
        last_position_ = new_position;
        return function(event, move);
    };

    release_event_ = observer.Bind(sf::Event::MouseButtonReleased, release_func, -100);
    press_event_ = observer.Bind(sf::Event::MouseButtonPressed, press_func, this->GetPosition().z + 1.0);
    move_event_ = observer.Bind(sf::Event::MouseMoved, move_func, this->GetPosition().z);
}

void Draggable3D::BindDrag(events::Observer& observer, const EVENT_FUNC& function, MouseButtons buttons) {
    auto func = [this, function](sf::Event event, glm::vec2 moved) {
        glm::vec3 move = math::to_world_coords(stage::StageManager::Instance().windowSize() / 2.0f + moved);
        return function(event, move);
    };

    Draggable::BindDrag(observer, func, buttons);
}

void Draggable3D::on_start(sf::Event event) {
	Draggable::on_start(event);
	start_point_info_ = stage::StageManager::Instance().Context()->PickingTexture.ReadPixel(start_position_.x, start_position_.y);
}
}  // namespace events
