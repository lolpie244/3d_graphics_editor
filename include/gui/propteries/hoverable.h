#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>
#include <memory>

#include "events/observer.h"
#include "gui/elements/base.h"

namespace gui::mixins {
class Hoverable : virtual public GuiElement {
   public:
    virtual ~Hoverable() = default;

    void BindMouseIn(events::Observer &observer, const events::EVENT_FUNC &function);
    void BindMouseOut(events::Observer &observer, const events::EVENT_FUNC &function);

    virtual bool Contains(sf::Vector2f point);
    virtual bool ContainsMouse(sf::Event event);

   protected:
    std::unique_ptr<events::Event> mouse_in_event_;
    std::unique_ptr<events::Event> mouse_out_event_;
    bool mouse_in_flag_ = false;
    bool check_transparecy_ = false;
};
}  // namespace gui::mixins
