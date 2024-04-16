#include "test_stage.h"

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

#include "gui/elements/button.h"
#include "gui/elements/buttons_list.h"
#include "gui/elements/text.h"
#include "gui/propteries/scaleable.h"
#include "stage/stage_manager.h"
#include "test_stage1/test_stage1.h"
#include "utils/texture.h"
#include "utils/vector2.h"

TestStage::TestStage() {
    events.push_back(observer_.KeyBind({sf::Keyboard::Space, sf::Keyboard::A}, [](sf::Event event) {
        stage::StageManager::Instance().NextStage(std::make_unique<TestStage1>());
        return true;
    }));
    events.push_back(observer_.KeyBind({sf::Keyboard::Escape}, [this](sf::Event event) {
        this->Stop(stage::StageState::Exit);
        return true;
    }));

    auto theme = utils::SvgTexture::loadFromFile("resources/theme.svg");
    auto button = std::make_shared<gui::Button>(utils::Vector2f(200, 100), utils::Vector2f(380, 94));
    button->Text().SetText(L"Help me");
    button->Text().SfText().setFillColor(sf::Color::Black);
    button->SetReleasedTexture({theme->getElement("g1"), {0, 0.01}, {0.2, 0.3}});
    button->SetPressedTexture({theme->getElement("g517"), {0, 0.1}, {0.2, 0.3}});

    button->BindRelease(observer_, [](sf::Event) {
        stage::StageManager::Instance().NextStage(std::make_unique<TestStage1>());
        return true;
    });

    elements_.Insert({button});
}

void TestStage::Run() { PollEvents(); }
