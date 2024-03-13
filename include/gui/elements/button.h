#pragma once

#include "gui/elements/base.h"
#include "gui/propteries/clickable.h"
namespace gui
{
class Button : virtual public BaseGuiElement, virtual public mixins::Clickable
{
public:
	Button();
	virtual ~Button() {}

private:
	void change_state();

public:
	enum State { RELEASED, PRESSED };
};
} // namespace gui
