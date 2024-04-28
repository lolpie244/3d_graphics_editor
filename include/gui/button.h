#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include "data/texture.h"
#include "events/propterties/clickable.h"
#include "events/propterties/scaleable.h"
#include "gui/sprite_element.h"
#include "gui/text.h"
#include "math/vector2.h"

namespace gui {

class Button;

class ButtonText : virtual public Text {
   public:
    ButtonText(Button* button);
    virtual void Resize(math::Vector2f size) override;
    virtual void SetPosition(float x, float y, float z) override;

    void SetOffset(math::Vector2f offset);
    void SetBordersSize(math::Vector2f border_size);

   private:
    math::Vector2f borders_size_{1, 1};
    math::Vector2f offset_{0, 0};
};

class Button : virtual public SpriteGuiElement, virtual public events::Clickable, virtual public events::Scaleable {
   public:
    struct TextureInfo {
        data::Texture texture;
        math::Vector2f offset = {0, 0};
        math::Vector2f borders_size = {0.1, 0.1};
    };

   public:
    Button();
    Button(glm::vec3 position, math::Vector2f size);

    virtual ~Button() = default;

    gui::ButtonText& Text();

    void BindPress(events::Observer& observer, const events::EVENT_FUNC& function,
                   MouseButtons buttons = {sf::Mouse::Left}) override;
    void OnRelease() override;

    void SetPressedTexture(TextureInfo texture);
    void SetReleasedTexture(TextureInfo texture);

   private:
    void SetTexture(data::Texture texture) override { SpriteGuiElement::SetTexture(texture); };
    void update_texture();

   protected:
    ButtonText text_;
    TextureInfo pressed_texture_;
    TextureInfo released_texture_;
};
}  // namespace gui
