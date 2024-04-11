#include "gui/elements_container.h"

#include "events/observer.h"
#include "gui/propteries/scaleable.h"

namespace gui {
void ElementContainer::BindScale(events::Observer& observer) {
    for (auto& element : elements_) {
        auto scalable = dynamic_cast<gui::mixins::Scaleable*>(element.get());
        if (scalable != nullptr)
            scalable->BindScale(observer);
    }
}

void ElementContainer::Insert(std::initializer_list<ElementType> new_elements) {
    elements_.insert(elements_.end(), new_elements);
}

void ElementContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto& element : elements_) { element->draw(target, states); }
}
}  // namespace gui
