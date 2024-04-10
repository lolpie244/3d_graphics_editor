#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "events/event.h"
#include "events/observer.h"
#include "gui/elements/base.h"

namespace gui::mixins {
class Scaleable : virtual public gui::GuiElement {
   public:
    virtual ~Scaleable() = default;
    void BindScale(events::Observer& observer);

   protected:
    virtual void scale(const utils::Vector2f& window_scale) = 0;
    utils::Vector2f oldScale();

   protected:
    std::unique_ptr<events::Event> event;

   private:
    utils::Vector2f old_scale_{1, 1};
};

// scale by width and height. The same as default scale
class DefaultScale : virtual public Scaleable {
   protected:
    void scale(const utils::Vector2f& window_scale) override;
};

// scale, but with saving texture proportions
class SaveProportionScale : virtual public Scaleable {
   protected:
    void scale(const utils::Vector2f& window_scale) override;
};
}  // namespace gui::mixins
