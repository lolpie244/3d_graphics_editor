#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "gui/elements/sprite_element.h"
#include "gui/elements/text.h"
#include "gui/propteries/clickable.h"
#include "gui/propteries/scaleable.h"
#include "utils/texture.h"
#include "utils/vector2.h"

namespace gui {

class Button;

class ButtonText : virtual public Text {
   public:
    ButtonText(Button* button);
    virtual void Resize(utils::Vector2f size) override;
    virtual void SetPosition(utils::Vector2f position) override;

    void SetOffset(utils::Vector2f offset);
    void SetBordersSize(utils::Vector2f border_size);

   private:
    utils::Vector2f borders_size_{1, 1};
    utils::Vector2f offset_{0, 0};
};

class Button : virtual public SpriteGuiElement, virtual public mixins::Clickable, virtual public mixins::Scaleable {
   public:
    struct TextureInfo {
        utils::Texture texture;
        utils::Vector2f offset = {0, 0};
        utils::Vector2f borders_size = {0.1, 0.1};
    };

   public:
    Button();
    Button(utils::Vector2f position, utils::Vector2f size);

    virtual ~Button() = default;

    gui::ButtonText& Text();

    void BindPress(events::Observer& observer, const events::EVENT_FUNC& function) override;
    void OnRelease() override;

    void SetPressedTexture(TextureInfo texture);
    void SetReleasedTexture(TextureInfo texture);

   private:
    void SetTexture(utils::Texture texture) override { SpriteGuiElement::SetTexture(texture); };
    void update_texture();

   protected:
    ButtonText text_;
    TextureInfo pressed_texture_;
    TextureInfo released_texture_;
};
}  // namespace gui
