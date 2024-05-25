#include "gui/radio_button.h"

#include "gui/button.h"

namespace gui {

void RadioButton::RadioButtonGroup::AddButton(RadioButton* button) { radio_buttons.push_back(button); }
void RadioButton::RadioButtonGroup::RemoveButton(RadioButton* button) { radio_buttons.remove(button); }

RadioButton::RadioButton(glm::vec3 position, glm::vec2 size) : RadioButton(position, size, NewGroup()) {}
RadioButton::RadioButton(glm::vec3 position, glm::vec2 size, GroupType group) : group_(group) {
    this->SetPosition(position.x, position.y, position.z);
    this->Resize(size);
	group_->AddButton(this);
}

RadioButton::~RadioButton() { group_->RemoveButton(this); }

void RadioButton::BindPress(events::Observer& observer, const events::EVENT_FUNC& function) {
    auto event_function = [this, function](sf::Event event) {
        UpdateTexture();
        group_->current = this;
        for (auto* button : group_->radio_buttons) {
            if (button != this)
                button->Release(event);
        }
        return function(event);
    };

    BindPress(observer, event_function, {sf::Mouse::Left});
}

void RadioButton::BindPress(events::Observer& observer, const events::EVENT_FUNC& function, MouseButtons buttons) {
    Button::BindPress(observer, function, buttons);
}

RadioButton::GroupType RadioButton::Group() { return group_; }
void RadioButton::SetGroup(GroupType new_group) {
    group_->RemoveButton(this);
    group_ = new_group;
	group_->AddButton(this);
}

}  // namespace gui
