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
#include "gui/elements/buttons_list.h"
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

	auto theme = utils::SvgTexture::loadFromFile("resources/theme1.svg");

	auto other_button_list = std::make_unique<gui::ButtonsList>();
	other_button_list->SetButtonTexture(theme->getElement("g4"));
	other_button_list->SetFontColor(sf::Color::Black);

	auto start = std::make_shared<gui::ButtonFromList>("Starting here");
	auto pause = std::make_shared<gui::ButtonFromList>("Pause");
	auto exit = std::make_shared<gui::ButtonFromList>("Exit");

	start->BindPress(observer_, [](sf::Event event) {std::cout << "start\n"; return true;});
	pause->BindPress(observer_, [](sf::Event event) {std::cout << "pause\n"; return true;});
	exit->BindPress(observer_, [](sf::Event event) {std::cout << "exit\n"; return true;});

	other_button_list->AddButtons({start, pause, exit});
	//////////////////////////////////////////////////////////////////////

	auto button_list = std::make_unique<gui::ButtonsList>(utils::Vector2f(200, 100), utils::Vector2f(120, 24));
	button_list->SetButtonTexture(theme->getElement("g4"));
	button_list->SetFontColor(sf::Color::Black);

	auto test = std::make_shared<gui::ButtonFromList>("Test");
	auto menu = std::make_shared<gui::ButtonFromList>("Menu");
	button_list->AddButtons({test, menu});

	menu->AddButtonList(observer_, other_button_list.get(), gui::ListOrientation::Horizontal);
	test->BindPress(observer_, [](sf::Event event) {std::cout << "test\n"; return true;});

	elements.push_back(std::move(button_list));
	elements.push_back(std::move(other_button_list));
}

void TestStage::Run() {
    PollEvents();

    window->clear();
    for (auto& element : elements) window->draw(*element);
    FrameEnd();
}
