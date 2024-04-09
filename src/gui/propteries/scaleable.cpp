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

namespace gui::mixins {
using settings::DEFAULT_RESOLUTION;

sf::Vector2f Scaleable::oldScale() { return old_scale_; }
void Scaleable::BindScale(events::Observer& observer) {
	sf::Vector2f size = (sf::Vector2f)stage::StageManager::Instance().window->getSize();

    auto scale_ = sf::Vector2f(size.x / DEFAULT_RESOLUTION.first, size.y / DEFAULT_RESOLUTION.second);
    this->scale(scale_);
    old_scale_ = scale_;

    auto event_function = [this](sf::Event event) {
        auto scale_ =
            sf::Vector2f(event.size.width / DEFAULT_RESOLUTION.first, event.size.height / DEFAULT_RESOLUTION.second);

        this->scale(scale_);
        old_scale_ = scale_;
        return false;
    };

    event = observer.Bind(sf::Event::Resized, event_function);
}

void DefaultScale::scale(const sf::Vector2f& window_scale) {
    auto current_scale = sf::Vector2f(window_scale.x / oldScale().x, window_scale.y / oldScale().y);

    this->Resize(sf::Vector2f(this->Size().x * current_scale.x, this->Size().y * current_scale.y));
    this->SetPosition(sf::Vector2f(this->Position().x * current_scale.x, this->Position().y * current_scale.y));
}
}  // namespace gui::mixins
