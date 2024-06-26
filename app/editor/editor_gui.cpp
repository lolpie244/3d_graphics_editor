#include <tinyfiledialogs.h>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <array>
#include <memory>

#include "data/texture.h"
#include "editor.h"
#include "editor/network/network.h"
#include "gui/buttons_list.h"
#include "gui/radio_button.h"
#include "render/model.h"
#include "utils/settings.h"

void EditorStage::InitGui() {
    selection_rect_ = std::make_shared<gui::SelectRect>();
    selection_rect_->Disable();

    auto theme = data::SvgTexture::loadFromFile("resources/theme.svg");

    auto menu_bar = std::make_shared<gui::ButtonsList>(glm::vec3(150, 20, 0), glm::vec2(300, 40), 0,
                                                       gui::ListOrientation::Horizontal);
    menu_bar->SetPressedTexture({theme->getElement("g4"), {0, 0.01}, {0.2, 0.3}});

    auto file_button_list = std::make_shared<gui::ButtonsList>();
    file_button_list->SetPressedTexture({theme->getElement("g4"), {0, 0.01}, {0.2, 0.3}});

    auto file_button = std::make_shared<gui::ButtonFromList>(L"Файл");
    auto figures_button = std::make_shared<gui::ButtonFromList>(L"Фігури");
    network_button_ = std::make_shared<gui::ButtonFromList>(L"Кімната");
    menu_bar->AddButtons({file_button, figures_button, network_button_});

    file_button->AddButtonList(observer_, file_button_list);
    auto new_file = std::make_shared<gui::ButtonFromList>(L"Новий файл");
    auto open_file = std::make_shared<gui::ButtonFromList>(L"Відкрити файл");
    auto save_file = std::make_shared<gui::ButtonFromList>(L"Зберегти");
    auto save_as_file = std::make_shared<gui::ButtonFromList>(L"Зберегти як");
    auto import_model = std::make_shared<gui::ButtonFromList>(L"Імпорт");
    file_button_list->AddButtons({new_file, open_file, save_file, save_as_file, import_model});

    new_file->BindPress(observer_, [this](sf::Event) { return NewScene(); });
    open_file->BindPress(observer_, [this](sf::Event) { return OpenScene(); });
    save_file->BindPress(observer_, [this](sf::Event) { return SaveScene(); });
    save_as_file->BindPress(observer_, [this](sf::Event) { return SaveAsScene(); });
    import_model->BindPress(observer_, [this](sf::Event) { return ImportModel(); });
    /////////////////////////////////////
    auto network_button_list = std::make_shared<gui::ButtonsList>();
    network_button_list->SetPressedTexture({theme->getElement("g4"), {0, 0.01}, {0.2, 0.3}});
    network_button_->AddButtonList(observer_, network_button_list);

    auto client = std::make_shared<gui::ButtonFromList>(L"Приєднатись");
    auto server = std::make_shared<gui::ButtonFromList>(L"Створити");
    network_button_list->AddButtons({client, server});
    client->BindPress(observer_, [this](sf::Event event) { return ClientButton(event); });
    server->BindPress(observer_, [this](sf::Event event) { return ServerButton(event); });
    ////////////////////////////////////
    auto figures_button_list = std::make_shared<gui::ButtonsList>();
    figures_button_list->SetPressedTexture({theme->getElement("g4"), {0, 0.01}, {0.2, 0.3}});
    figures_button->AddButtonList(observer_, figures_button_list);

    for (auto &[name, filename] : default_figures_) {
        auto button = std::make_shared<gui::ButtonFromList>(name);

        button->BindPress(observer_, [this, &filename](sf::Event) {
            auto model = render::Model::loadFromFile(filename, DEFAULT_MODEL_CONFIG);
            AddModel(std::move(model));
            return true;
        });

        figures_button_list->AddButton(button);
    }
    auto light_button = std::make_shared<gui::ButtonFromList>(L"Джерело світла");
    figures_button_list->AddButton(light_button);
    light_button->BindPress(observer_, [this](sf::Event event) {
        AddLight(event);
        return true;
    });

    //////////////////////////////////////////////////////
    auto mode_group = gui::RadioButton::NewGroup();

    int i = 0;

    for (auto &[name, mode] : draw_modes_) {
        auto button =
            std::make_shared<gui::RadioButton>(glm::vec3(1650 + 90 * (i++), 70, 0), glm::vec2(82, 82), mode_group);

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
    }

    auto gizmo_button_list = std::make_shared<gui::ButtonsList>(glm::vec3(60, 200, 0), glm::vec2(82, 82), 10);
    gizmo_button_list->SetReleasedTexture({theme->getElement("g582"), {0, 0.01}, {0.2, 0.3}});
    gizmo_button_list->SetPressedTexture({theme->getElement("g583"), {0, 0.1}, {0.2, 0.3}});
    for (auto &[name, mode] : gizmos_) {
        auto button = std::make_shared<gui::ButtonFromList>(name);

        button->BindPress(observer_, [&mode, this](sf::Event event) {
            mode->SetModel(current_gizmo_->GetModel());
            current_gizmo_->Reset();
            current_gizmo_ = mode.get();
            return true;
        });

        mode->BindDrag(observer_, [this, &mode](sf::Event event, glm::vec3 moved) {
            mode->MouseMove({event.mouseMove.x, event.mouseMove.y}, mode->PressInfo().Type);
            SendRequest(after_gizmo_transform_);

            return true;
        });

        mode->BindPress(observer_, [this, &mode](sf::Event event) {
            mode->MousePress();
            return true;
        });

        gizmo_button_list->AddButton(button);
    }

    elements_.Insert({selection_rect_, gizmo_button_list, menu_bar});
}
