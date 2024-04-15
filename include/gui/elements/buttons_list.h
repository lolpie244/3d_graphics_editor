#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <initializer_list>
#include <memory>
#include <vector>

#include "gui/elements/base.h"
#include "gui/elements/button.h"
#include "gui/elements/sprite_element.h"
#include "gui/propteries/hoverable.h"
#include "gui/propteries/scaleable.h"
#include "utils/texture.h"
#include "utils/vector2.h"

namespace gui {
enum ListOrientation { Horizontal, Vertical };

class ButtonsList;

class ButtonFromList : virtual public Button {
    friend ButtonsList;

   public:
    ButtonFromList() = default;
    ButtonFromList(const sf::String& text);

    void AddButtonList(events::Observer& observer, ButtonsList* button_list, ListOrientation orientation);

   private:
    virtual void Resize(utils::Vector2f size) override { Button::Resize(size); };
    virtual void SetPosition(utils::Vector2f position) override { Button::SetPosition(position); };
};

class ButtonsList : public virtual GuiElement, virtual public mixins::Scaleable, public virtual mixins::Hoverable {
    using ButtonType = std::shared_ptr<ButtonFromList>;
    friend ButtonFromList;

   public:
    ButtonsList(ListOrientation orientation = ListOrientation::Vertical);
    ButtonsList(utils::Vector2f position, utils::Vector2f size,
                ListOrientation orientation = ListOrientation::Vertical);

    sf::Rect<float> Rect() const override;
    void SetPosition(utils::Vector2f position) override;

    void AddButton(ButtonType button);
    void AddButtons(const std::initializer_list<ButtonType>& buttons);

    void SetFontColor(sf::Color color);
    void SetReleasedTexture(Button::TextureInfo texture);
    void SetPressedTexture(Button::TextureInfo texture);
    void SetOrientation(ListOrientation orientation);

   private:
    utils::Vector2f PositionCoef() const;

   private:
    std::vector<ButtonType> buttons_;
    ListOrientation orientation_;
    sf::Color font_color_;

	Button::TextureInfo released_texture_;
    Button::TextureInfo pressed_texture_;
};
}  // namespace gui
