#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <initializer_list>
#include <memory>
#include <vector>

#include "data/texture.h"
#include "events/propterties/hoverable.h"
#include "events/propterties/scaleable.h"
#include "gui/base.h"
#include "gui/button.h"
#include "gui/sprite_element.h"

namespace gui {
enum ListOrientation { Horizontal, Vertical };

class ButtonsList;

class ButtonFromList : virtual public Button {
    friend ButtonsList;

   public:
    ButtonFromList() = default;
    ButtonFromList(const sf::String& text);

    void Enable() override;

    void AddButtonList(events::Observer& observer, std::shared_ptr<ButtonsList> button_list);

   private:
    virtual void Resize(glm::vec2 size) override { Button::Resize(size); };
    virtual void Move(float x = 0, float y = 0, float z = 0) override { Button::Move(x, y, z); };

   private:
    std::shared_ptr<ButtonsList> button_list_;
};

class ButtonsList : public virtual GuiElement, virtual public events::Scaleable, public virtual events::Hoverable {
    using ButtonType = std::shared_ptr<ButtonFromList>;
    friend ButtonFromList;

   public:
    ButtonsList(float space = 0, ListOrientation orientation = ListOrientation::Vertical);
    ButtonsList(glm::vec3 position, glm::vec2 size, float space = 0,
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
    glm::vec2 PositionCoef() const;

   private:
    std::vector<ButtonType> buttons_;
    ListOrientation orientation_;
    sf::Color font_color_;

    float space_ = 0;

    Button::TextureInfo released_texture_;
    Button::TextureInfo pressed_texture_;
};
}  // namespace gui
