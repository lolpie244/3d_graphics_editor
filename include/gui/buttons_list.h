#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <initializer_list>
#include <memory>
#include <vector>

#include "gui/base.h"
#include "gui/button.h"
#include "gui/sprite_element.h"
#include "events/propterties/hoverable.h"
#include "events/propterties/scaleable.h"
#include "data/texture.h"
#include "math/vector2.h"

namespace gui {
enum ListOrientation { Horizontal, Vertical };

class ButtonsList;

class ButtonFromList : virtual public Button {
    friend ButtonsList;

   public:
    ButtonFromList() = default;
    ButtonFromList(const sf::String& text);

    void AddButtonList(events::Observer& observer, ButtonsList* button_list);

   private:
    virtual void Resize(math::Vector2f size) override { Button::Resize(size); };
    virtual void Move(float x = 0, float y = 0, float z = 0) override { Button::Move(x, y, z); };
};

class ButtonsList : public virtual GuiElement, virtual public events::Scaleable, public virtual events::Hoverable {
    using ButtonType = std::shared_ptr<ButtonFromList>;
    friend ButtonFromList;

   public:
    ButtonsList(ListOrientation orientation = ListOrientation::Vertical);
    ButtonsList(glm::vec3 position, math::Vector2f size,
                ListOrientation orientation = ListOrientation::Vertical);

    sf::Rect<float> Rect() const override;
    void Move(float x = 0, float y = 0, float z = 0) override;

    void AddButton(ButtonType button);
    void AddButtons(const std::initializer_list<ButtonType>& buttons);

    void SetFontColor(sf::Color color);
    void SetReleasedTexture(Button::TextureInfo texture);
    void SetPressedTexture(Button::TextureInfo texture);
    void SetOrientation(ListOrientation orientation);

   private:
    math::Vector2f PositionCoef() const;

   private:
    std::vector<ButtonType> buttons_;
    ListOrientation orientation_;
    sf::Color font_color_;

	Button::TextureInfo released_texture_;
    Button::TextureInfo pressed_texture_;
};
}  // namespace gui
