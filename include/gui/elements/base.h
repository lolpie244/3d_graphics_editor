#pragma once

#include <utility>
#include <memory>

namespace gui
{
class BaseGuiElement
{
public:
	BaseGuiElement();
	virtual ~BaseGuiElement();

	int get_id();

	virtual void enable() = 0;
	virtual void disable() = 0;
protected:
	int id_ = max_object_id++;
	bool is_active = true;

private:
	static int max_object_id;
};
} // namespace gui
