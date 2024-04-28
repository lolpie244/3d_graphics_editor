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
    auto button = std::make_shared<gui::Button>(glm::vec3(200, 100, 0), math::Vector2f(380, 94));
    button->Text().SetText(L"Help me");
    button->Text().SfText().setFillColor(sf::Color::Black);
    button->SetReleasedTexture({theme->getElement("g1"), {0, 0.01}, {0.2, 0.3}});
    button->SetPressedTexture({theme->getElement("g517"), {0, 0.1}, {0.2, 0.3}});

    button->BindRelease(observer_, [](sf::Event) {
        stage::StageManager::Instance().NextStage(std::make_unique<EditorStage>());
        return true;
    });

    elements_.Insert({button});
}

void MenuStage::Run() { PollEvents(); }