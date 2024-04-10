#include "gui/propteries/scaleable.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

#include "events/observer.h"
#include "gui/elements/sprite_element.h"
#include "stage/stage.h"
#include "stage/stage_manager.h"
#include "utils/settings.h"
#include "utils/vector2.h"

namespace gui::mixins {
using settings::DEFAULT_RESOLUTION;

utils::Vector2f Scaleable::oldScale() { return old_scale_; }

void Scaleable::Scale(const utils::Vector2f& window_size) {
    auto scale_ = window_size / DEFAULT_RESOLUTION;
    scale_method_->scale(this, scale_);
    old_scale_ = scale_;
}

void Scaleable::BindScale(events::Observer& observer) {
    Scale((utils::Vector2f)stage::StageManager::Instance().window->getSize());

    auto event_function = [this](sf::Event event) {
        Scale(utils::Vector2f(event.size.width, event.size.height));
        return false;
    };

    event = observer.Bind(sf::Event::Resized, event_function);
}

void DefaultScale::scale(Scaleable* obj, const utils::Vector2f& window_scale) {
    auto current_scale = window_scale / obj->oldScale();

    obj->Resize(utils::Vector2f(obj->Size() * current_scale));
    obj->SetPosition(utils::Vector2f(obj->Position() * current_scale));
}

void SaveProportionScale::scale(Scaleable* obj, const utils::Vector2f& window_scale) {
    auto position_scale = window_scale / obj->oldScale();
    auto size_scale = std::min(window_scale.x, window_scale.y) / std::min(obj->oldScale().x, obj->oldScale().y);

    obj->Resize(utils::Vector2f(obj->Size() * size_scale));
    obj->SetPosition(utils::Vector2f(obj->Position() * position_scale));
}
}  // namespace gui::mixins
