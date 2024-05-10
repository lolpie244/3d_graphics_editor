#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "base.h"
#include "data/texture.h"
#include "events/propterties/hoverable.h"

namespace gui {
class SpriteGuiElement : virtual public GuiElement, virtual public events::Hoverable {
   public:
    SpriteGuiElement(glm::vec3 position, glm::vec2 size);
    SpriteGuiElement() = default;
    virtual ~SpriteGuiElement() = default;

    sf::Rect<float> Rect() const override;

    virtual void Move(float x = 0, float y = 0, float z = 0) override;
	virtual void Resize(glm::vec2 size) override;
    virtual void SetTexture(data::Texture new_texture);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   protected:
    sf::Sprite sprite_;
    sf::Texture texture_;
    data::Texture svg_texture_;
};
}  // namespace gui
