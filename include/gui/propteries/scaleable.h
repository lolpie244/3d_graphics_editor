#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

#include "events/event.h"
#include "events/observer.h"
#include "gui/elements/base.h"
#include "utils/vector2.h"

namespace gui::mixins {

class Scaleable;

// no scale at all
class NoScale {
   public:
    virtual void scale(Scaleable* obj, const utils::Vector2f& window_scale) {};
};

class Scaleable : virtual public gui::GuiElement {
   public:
    virtual ~Scaleable() = default;
    void BindScale(events::Observer& observer);
	
	utils::Vector2f oldScale();

    template <typename T>
    void SetScale() {
		scale_method_ = std::make_unique<T>();
	}

    void Scale(const utils::Vector2f& window_size);
   protected:
    std::unique_ptr<events::Event> event;
    std::unique_ptr<NoScale> scale_method_ = std::make_unique<NoScale>();

   private:
    utils::Vector2f old_scale_{1, 1};
};

// scale by width and height. The same as default scale
class DefaultScale : virtual public NoScale {
   protected:
    void scale(Scaleable* obj, const utils::Vector2f& window_scale) override;
};

// scale, but with saving texture proportions
class SaveProportionScale : virtual public NoScale {
   protected:
    void scale(Scaleable* obj, const utils::Vector2f& window_scale) override;
};
}  // namespace gui::mixins
