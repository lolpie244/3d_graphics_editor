#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <future>
#include <memory>
#include <utility>

#include "events/propterties/hoverable.h"
#include "math/transform.h"
#include "utils/active.h"
#include "utils/tree.h"
#include "utils/uuid.h"

namespace gui {
class GuiElement : public Tree,
                   public sf::Drawable,
                   virtual public UUID,
                   virtual public Activatable,
                   virtual public math::Transform,
                   virtual public events::Hoverable {
   public:
    virtual ~GuiElement() = default;

    glm::vec2 Size() const;

    virtual sf::Rect<float> Rect() const = 0;

    virtual void Enable() override;
    virtual void Disable() override;

    virtual void Move(float x = 0, float y = 0, float z = 0) override;
    virtual void Scale(float x = 1, float y = 1, float z = 1) override;
    virtual void Resize(glm::vec2 size);

    virtual bool Contains(glm::vec2 point) override;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   protected:
    glm::vec2 size_;
};
}  // namespace gui
