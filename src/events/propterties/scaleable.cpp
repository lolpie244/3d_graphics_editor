#include "events/propterties/scaleable.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

#include "events/observer.h"
#include "math/vector2.h"
#include "stage/stage.h"
#include "stage/stage_manager.h"
#include "utils/settings.h"

namespace events {
using settings::DEFAULT_RESOLUTION;

math::Vector2f Scaleable::oldScale() { return old_scale_; }

void Scaleable::window_scale(const math::Vector2f& window_size) {
    auto scale_ = window_size / DEFAULT_RESOLUTION;
    scale_method_->scale(this, scale_);
    old_scale_ = scale_;
}

void Scaleable::BindScale(events::Observer& observer) {
    window_scale(stage::StageManager::Instance().windowSize());

    auto event_function = [this](sf::Event event) {
        window_scale(math::Vector2f(event.size.width, event.size.height));
        return false;
    };

    event = observer.Bind(sf::Event::Resized, event_function);
}

void DefaultScale::scale(Scaleable* obj, const math::Vector2f& window_scale) {
    auto current_scale = window_scale / obj->oldScale();
    auto new_position = math::Vector2f(obj->Position()) * current_scale;

    obj->Scale(current_scale.x, current_scale.y);
    obj->SetPosition(new_position.x, new_position.y, obj->Position().z);
}

void SaveProportionScale::scale(Scaleable* obj, const math::Vector2f& window_scale) {
    auto size_scale = std::min(window_scale.x, window_scale.y) / std::min(obj->oldScale().x, obj->oldScale().y);
    auto new_position = math::Vector2f(obj->Position()) * (window_scale / obj->oldScale());

    obj->Scale(size_scale);
    obj->SetPosition(new_position.x, new_position.y, obj->Position().z);
}
}  // namespace events
