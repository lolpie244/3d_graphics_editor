#include "data/texture.h"
#include "editor.h"
#include "editor/network/network.h"
#include "gui/buttons_list.h"
#include "gui/radio_button.h"

void EditorStage::InitGui() {
    selection_rect_ = std::make_shared<gui::SelectRect>();
    selection_rect_->Disable();

    auto theme = data::SvgTexture::loadFromFile("resources/theme.svg");

    auto mode_group = gui::RadioButton::NewGroup();

    int i = 0;

    for (auto& [name, mode] : draw_modes_) {
        auto button =
            std::make_shared<gui::RadioButton>(glm::vec3(1648 + 90 * i, 70, 0), glm::vec2(82, 82), mode_group);

        button->Text().SetText(name);
        button->Text().SfText().setFillColor(sf::Color::Black);

        button->BindPress(observer_, [&mode, this](sf::Event event) {
            current_draw_mode_ = mode.get();
            ClearSelection();
            return true;
        });

        button->SetReleasedTexture({theme->getElement("g582"), {0, 0.01}, {0.2, 0.3}});
        button->SetPressedTexture({theme->getElement("g583"), {0, 0.1}, {0.2, 0.3}});

        elements_.Insert({button});
        i++;
    }

    auto gizmo_button_list = std::make_shared<gui::ButtonsList>(glm::vec3(90, 70, 0), glm::vec2(82, 82));
    gizmo_button_list->SetReleasedTexture({theme->getElement("g582"), {0, 0.01}, {0.2, 0.3}});
    gizmo_button_list->SetPressedTexture({theme->getElement("g583"), {0, 0.1}, {0.2, 0.3}});
    for (auto& [name, mode] : gizmos_) {
        auto button = std::make_shared<gui::ButtonFromList>(name);

        button->BindPress(observer_, [&mode, this](sf::Event event) {
            mode->SetModel(current_gizmo_->GetModel());
            current_gizmo_->Reset();
            current_gizmo_ = mode.get();
            return true;
        });

        mode->BindDrag(observer_, [this, &mode](sf::Event event, glm::vec3 moved) {
            mode->MouseMove({event.mouseMove.x, event.mouseMove.y},
                            2.0f * moved * Scale() * settings::MOUSE_SENSATIVITY, mode->PressInfo().Type);
            return true;
        });

        mode->BindPress(observer_, [this, &mode](sf::Event event) {
            mode->MousePress();
            return true;
        });

        gizmo_button_list->AddButton(button);
    }

    elements_.Insert({selection_rect_, gizmo_button_list});
}
