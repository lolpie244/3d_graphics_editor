#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <initializer_list>
#include <unordered_set>

#include "events/observer.h"
#include "hoverable.h"

#include "render/opengl/picking_texture.h"

namespace events {
class Clickable : virtual public Hoverable {
   protected:
    static constexpr std::initializer_list<sf::Mouse::Button> all_buttons_ = {
        sf::Mouse::Left, sf::Mouse::Right, sf::Mouse::Middle, sf::Mouse::XButton1, sf::Mouse::XButton2,
    };

    typedef const std::unordered_set<sf::Mouse::Button>& MouseButtons;
    typedef const std::unordered_set<sf::Mouse::Button>& MouseButtonsType;

   public:
    virtual ~Clickable() = default;

    bool ContainsMouse(sf::Event event);

    virtual void BindPress(events::Observer& observer, const events::EVENT_FUNC& function,
                           MouseButtons buttons = all_buttons_);
    virtual void BindRelease(events::Observer& observer, const events::EVENT_FUNC& function,
                             MouseButtons buttons = all_buttons_);

    virtual void OnPress(sf::Event event) {}; // before buttons checking
    virtual void OnRelease(sf::Event event) {};

    bool IsPressed() const { return pressed_; }
	// glm::vec2 StartPosition() { };

   protected:
    events::Event press_event_;
    events::Event release_event_;

    bool pressed_ = false;
};

class Clickable3D : virtual public Clickable, virtual public Hoverable3D {
   public:
    virtual void BindPress(events::Observer& observer, const events::EVENT_FUNC& function,
                           MouseButtons buttons = all_buttons_) override;

    render::PickingTexture::Info PressInfo();

   private:
    render::PickingTexture::Info pixel_info_;
};
}  // namespace events
