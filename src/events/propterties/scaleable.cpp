#include "events/propterties/scaleable.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

#include "events/observer.h"
#include "stage/stage.h"
#include "stage/stage_manager.h"
#include "utils/settings.h"

namespace events {
using settings::DEFAULT_RESOLUTION;

glm::vec2 Scaleable::oldScale() { return old_scale_; }

void Scaleable::BindScale(events::Observer& observer) {
    auto event_function = [this](sf::Event event) {
        auto scale_ = glm::vec2(event.size.width, event.size.height) / DEFAULT_RESOLUTION;
        scale_method_->scale(this, scale_);
        old_scale_ = scale_;
        return false;
    };

    event = observer.Bind(sf::Event::Resized, event_function);
}

void DefaultScale::scale(Scaleable* obj, const glm::vec2& window_scale) {
    auto current_scale = window_scale / obj->oldScale();
    auto new_position = glm::vec2(obj->GetPosition()) * current_scale;

    obj->Scale(current_scale.x, current_scale.y);
    obj->SetPosition(new_position.x, new_position.y, obj->GetPosition().z);
}

void SaveProportionScale::scale(Scaleable* obj, const glm::vec2& window_scale) {
    auto size_scale = std::min(window_scale.x, window_scale.y) / std::min(obj->oldScale().x, obj->oldScale().y);
    auto new_position = glm::vec2(obj->GetPosition()) * (window_scale / obj->oldScale());

    obj->Scale(size_scale);
    obj->SetPosition(new_position.x, new_position.y, obj->GetPosition().z);
}
}  // namespace events
