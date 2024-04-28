#include "events/propterties/clickable.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include "events/event.h"
#include "events/observer.h"
#include "stage/stage_manager.h"
#include "utils/active.h"

namespace events {
void Clickable::BindPress(events::Observer& observer, const events::EVENT_FUNC& function, MouseButtons buttons) {
    auto event_function = [this, function, buttons](sf::Event event) {
        ReturnOnDisable(false);
        if (!this->ContainsMouse(event))
            return false;

        OnPress(event);
        if (!buttons.contains(event.mouseButton.button))
            return false;

        pressed_ = true;
        return function(event);
    };

    press_event_ = observer.Bind(sf::Event::MouseButtonPressed, event_function, this->GetPosition().z);

    if (!release_event_)
        BindRelease(observer, [](sf::Event) { return false; }, buttons);
}

bool Clickable::ContainsMouse(sf::Event event) {
    return this->Contains(glm::vec2(event.mouseButton.x, event.mouseButton.y));
}

void Clickable::BindRelease(events::Observer& observer, const events::EVENT_FUNC& function, MouseButtons buttons) {
    auto event_function = [this, function, buttons](sf::Event event) {
        if (!pressed_)
            return false;
        pressed_ = false;
        OnRelease(event);

        ReturnOnDisable(false);

        if (!this->ContainsMouse(event) || !buttons.contains(event.mouseButton.button))
            return false;
        return function(event);
    };

    release_event_ = observer.Bind(sf::Event::MouseButtonReleased, event_function, this->GetPosition().z);

    if (!press_event_)
        BindPress(observer, [](sf::Event) { return true; }, buttons);
}

void Clickable3D::BindPress(events::Observer& observer, const events::EVENT_FUNC& function, MouseButtons buttons) {
    auto func = [this, function](sf::Event event) {
        this->pixel_info_ = stage::StageManager::Instance().Context()->PickingTexture.ReadPixel(event.mouseButton.x,
                                                                                                event.mouseButton.y);
        return function(event);
    };
    Clickable::BindPress(observer, func, buttons);
}
render::PickingTexture::Info Clickable3D::PressInfo() {
    if (this->IsPressed())
        return this->pixel_info_;
    return {0, 0, 0};
}

}  // namespace events
