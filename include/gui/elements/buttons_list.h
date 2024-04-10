#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <initializer_list>
#include <memory>
#include <vector>

#include "gui/elements/base.h"
#include "gui/elements/button.h"
#include "gui/elements/sprite_element.h"
#include "gui/propteries/scaleable.h"
#include "utils/texture.h"
#include "utils/vector2.h"

namespace gui {

class ButtonsList;

class ButtonFromList : virtual public Button {
    friend ButtonsList;

   public:
	ButtonFromList() = default;
	ButtonFromList(const sf::String& text);

    void SetPosition(utils::Vector2f position) override;

   private:
    size_t id_;
};

class ButtonsList : public virtual GuiElement, public virtual mixins::DefaultScale {
   public:
    ButtonsList(utils::Vector2f position, utils::Vector2f size);

	utils::Vector2f LeftCorner() const override;

    void AddButton(const std::string& name, std::unique_ptr<ButtonFromList> button);
    void AddButton(const std::string& name, const sf::String& text="");

    void SetFontColor(sf::Color color);
    void SetButtonTexture(std::shared_ptr<utils::Texture> texture);

	ButtonFromList* Button(const std::string& name);


   private:
    std::unordered_map<std::string, std::unique_ptr<ButtonFromList>> buttons_;
    sf::Color font_color_;

    std::shared_ptr<utils::Texture> button_texture_;
};
}  // namespace gui
