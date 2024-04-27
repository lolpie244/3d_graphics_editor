#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <initializer_list>
#include <vector>

#include "events/observer.h"
#include "gui/base.h"

namespace gui {
typedef std::shared_ptr<GuiElement> ElementType;

class ElementContainer : public sf::Drawable {
   public:
    void BindScale(events::Observer&);
    void Insert(std::initializer_list<ElementType>);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

   private:
    std::vector<ElementType> elements_;
};
}  // namespace gui
