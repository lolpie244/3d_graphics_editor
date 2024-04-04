#include "gui/propteries/drawable.h"

namespace gui::mixins {

void SpriteDraw::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (is_active_)
        target.draw(this->sprite_, states);
}
}  // namespace gui::mixins
