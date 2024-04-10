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
void Scaleable::BindScale(events::Observer& observer) {
    utils::Vector2f size = (utils::Vector2f)stage::StageManager::Instance().window->getSize();

    auto scale_ = size / DEFAULT_RESOLUTION;
    this->scale(scale_);
    old_scale_ = scale_;

    auto event_function = [this](sf::Event event) {
        utils::Vector2f scale_ = utils::Vector2f(event.size.width, event.size.height) / DEFAULT_RESOLUTION;

        this->scale(scale_);
        old_scale_ = scale_;
        return false;
    };

    event = observer.Bind(sf::Event::Resized, event_function);
}

void DefaultScale::scale(const utils::Vector2f& window_scale) {
    auto current_scale = window_scale / oldScale();

    this->Resize(utils::Vector2f(this->Size() * current_scale));
    this->SetPosition(utils::Vector2f(this->Position() * current_scale));
}

void SaveProportionScale::scale(const utils::Vector2f& window_scale) {
    auto position_scale = window_scale / oldScale();
    auto size_scale = std::min(window_scale.x, window_scale.y) / std::min(oldScale().x, oldScale().y);

    this->Resize(utils::Vector2f(this->Size() * size_scale));
    this->SetPosition(utils::Vector2f(this->Position() * position_scale));
}
}  // namespace gui::mixins
