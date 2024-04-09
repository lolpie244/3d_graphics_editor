#include "test_stage.h"

#include <lunasvg.h>

#include <SFML/Graphics/CircleShape.hpp>
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
#include "gui/elements/text.h"
#include "gui/propteries/scaleable.h"
#include "stage/stage_manager.h"
#include "test_stage1/test_stage1.h"
#include "utils/texture.h"

TestStage::TestStage() {
    events.push_back(observer_.KeyBind({sf::Keyboard::Space, sf::Keyboard::A}, [](sf::Event event) {
        stage::StageManager::Instance().NextStage(std::make_unique<TestStage1>());
        return true;
    }));
    events.push_back(observer_.KeyBind({sf::Keyboard::Escape}, [this](sf::Event event) {
        this->Stop(stage::StageState::Exit);
        return true;
    }));

    std::unique_ptr<gui::Button> but = std::make_unique<gui::Button>(sf::Vector2f(200, 200), sf::Vector2f(200, 100));
    auto theme = utils::SvgTexture::loadFromFile("resources/theme1.svg");
    but->SetTexture(theme->getElement("g548"));

    but->Text().SetText(L"Тест");
    but->Text().SfText().setFillColor(sf::Color::White);
    but->BindPress(observer_, [](sf::Event event) {
        stage::StageManager::Instance().NextStage(std::make_unique<TestStage1>());
        return true;
    });

    elements.push_back(std::move(but));

    for (auto& element : elements) {
        auto scalable = dynamic_cast<gui::mixins::Scaleable*>(element.get());
        if (scalable != nullptr)
            scalable->BindScale(observer_);
    }
}

void TestStage::Run() {
    PollEvents();

    window->clear();
    for (auto& element : elements) window->draw(*element);
    FrameEnd();
}
