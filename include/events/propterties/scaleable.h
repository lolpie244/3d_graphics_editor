#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

#include "events/event.h"
#include "events/observer.h"
#include "math/transform.h"

#include "utils/active.h"
namespace events {

class Scaleable;

// no scale at all
class NoScale {
   public:
    virtual void scale(Scaleable* obj, const glm::vec2& window_scale) {};
};

class Scaleable : virtual public math::Transform, virtual public Activatable {
   public:
    virtual ~Scaleable() = default;
    void BindScale(events::Observer& observer);

    glm::vec2 oldScale();

    template <typename T>
    void SetScaleMethod() {
        scale_method_ = std::make_unique<T>();
    }

   protected:
    events::Event event;
    std::unique_ptr<NoScale> scale_method_ = std::make_unique<NoScale>();

   private:
    glm::vec2 old_scale_{1, 1};
};

// scale by width and height. The same as default scale
class DefaultScale : virtual public NoScale {
   protected:
    void scale(Scaleable* obj, const glm::vec2& window_scale) override;
};

// scale, but with saving texture proportions
class SaveProportionScale : virtual public NoScale {
   protected:
    void scale(Scaleable* obj, const glm::vec2& window_scale) override;
};
}  // namespace events
