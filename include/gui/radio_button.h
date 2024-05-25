#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cassert>
#include <list>
#include <memory>

#include "data/texture.h"
#include "events/propterties/clickable.h"
#include "gui/button.h"

namespace gui {

class RadioButton : public Button {
    struct RadioButtonGroup {
        std::list<RadioButton*> radio_buttons;
        RadioButton* current = nullptr;

		void AddButton(RadioButton* button);
        void RemoveButton(RadioButton* button);
    };

   public:
    typedef std::shared_ptr<RadioButtonGroup> GroupType;
    static GroupType NewGroup() { return std::make_shared<RadioButtonGroup>(); }

   public:
    RadioButton(glm::vec3 position, glm::vec2 size);
    RadioButton(glm::vec3 position, glm::vec2 size, GroupType group);
    ~RadioButton();

    void BindPress(events::Observer& observer, const events::EVENT_FUNC& function);

    GroupType Group();
    void SetGroup(GroupType group);

   protected:
    using events::Clickable::BindRelease;
    void OnRelease(sf::Event) override{};
    void Release(sf::Event event ) { Button::OnRelease(event); };

    void BindPress(events::Observer& observer, const events::EVENT_FUNC& function, MouseButtons buttons) override;

   private:
    GroupType group_;
};
}  // namespace gui
