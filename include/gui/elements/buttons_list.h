#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "gui/elements/base.h"
#include "gui/elements/button.h"

namespace gui {
class ButtonsList : public virtual GuiElement {
   public:
    void AddButtons(std::initializer_list<std::unique_ptr<Button>>);
    void AddButton(std::unique_ptr<Button>);

   private:
    std::vector<std::unique_ptr<Button>> buttons_;
};
}  // namespace gui
