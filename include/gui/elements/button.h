#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "gui/elements/sprite_element.h"
#include "gui/propteries/clickable.h"
namespace gui
{
class Button : virtual public SpriteGuiElement, virtual public mixins::Clickable
{
public:
	Button();
	virtual ~Button() {}

	void BindPress(events::Observer& observer, const events::EVENT_FUNC& function) override;
    void OnRelease() override;

private:
	void update_texture();

private:
	sf::Texture pressed_texture_;
};
} // namespace gui
