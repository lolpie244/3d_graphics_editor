#include "menu.h"

#include <lunasvg.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <iostream>
#include <memory>

#include "data/texture.h"
#include "editor/editor.h"
#include "events/propterties/scaleable.h"
#include "gui/button.h"
#include "gui/buttons_list.h"
#include "gui/text.h"
#include "stage/stage_manager.h"

MenuStage::MenuStage() {
    events.push_back(observer_.KeyBind({sf::Keyboard::Space, sf::Keyboard::A}, [](sf::Event event) {
        stage::StageManager::Instance().NextStage(std::make_unique<EditorStage>());
        return true;
    }));
    events.push_back(observer_.KeyBind({sf::Keyboard::Escape}, [this](sf::Event event) {
        this->Stop(stage::StageState::Exit);
        return true;
    }));

    auto theme = data::SvgTexture::loadFromFile("resources/theme.svg");
    auto button = std::make_shared<gui::Button>(glm::vec3(200, 100, 0), glm::vec2(380, 94));
    button->Text().SetText(L"Кнопка");
    button->Text().SfText().setFillColor(sf::Color::Black);
    button->SetReleasedTexture({theme->getElement("g1"), {0, 0.01}, {0.2, 0.3}});
    button->SetPressedTexture({theme->getElement("g517"), {0, 0.1}, {0.2, 0.3}});

    button->BindRelease(observer_, [](sf::Event) {
        stage::StageManager::Instance().NextStage(std::make_unique<EditorStage>());
        return true;
    });

    auto button_list = std::make_shared<gui::ButtonsList>(glm::vec3(900, 300, 0), glm::vec2(380, 94));

    auto test = std::make_shared<gui::ButtonFromList>(L"");
    auto test1 = std::make_shared<gui::ButtonFromList>(L"󱎖");
    auto test2 = std::make_shared<gui::ButtonFromList>(L"");

    auto test3 = std::make_shared<gui::ButtonFromList>(L"Кнопка 3");
    auto test4 = std::make_shared<gui::ButtonFromList>(L"Кнопка 4");
    button_list->AddButtons({test, test1, test2});

    auto button_list2 =
        std::make_shared<gui::ButtonsList>(glm::vec3(300, 300, 0), glm::vec2(380, 94), gui::ListOrientation::Vertical);
    button_list2->AddButtons({test3, test4});

    test2->AddButtonList(observer_, button_list2.get());

    button_list->SetReleasedTexture({theme->getElement("g1"), {0, 0.01}, {0.2, 0.3}});
    button_list2->SetReleasedTexture({theme->getElement("g1"), {0, 0.01}, {0.2, 0.3}});
    elements_.Insert({button, button_list, button_list2});
}

void MenuStage::Run() { PollEvents(); }
