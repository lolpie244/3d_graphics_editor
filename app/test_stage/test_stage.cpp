#include "test_stage.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <memory>

#include "gui/elements/text.h"
#include "stage/stage_manager.h"
#include "test_stage1/test_stage1.h"

TestStage::TestStage() {
    events.push_back(observer_.KeyBind({sf::Keyboard::Space, sf::Keyboard::A}, [](sf::Event event) {
        stage::StageManager::Instance().NextStage(std::make_unique<TestStage1>());
        return true;
    }));
    events.push_back(observer_.KeyBind({sf::Keyboard::Escape}, [this](sf::Event event) {
        this->Stop(stage::StageState::Exit);
        return true;
    }));
	std::unique_ptr<gui::Text> a = std::make_unique<gui::Text>(sf::Vector2f(100, 100), sf::Vector2f(200, 200));

	a->SfText().setFillColor(sf::Color::Red);
	a->SetText("Help me");
	a->SetFontSize(50);
    elements.push_back(std::move(a));
}

void TestStage::Run() {
    PollEvents();
    sf::CircleShape shape(50);

    shape.setFillColor(sf::Color(100, 250, 50));

    window->clear();
    window->draw(shape);
    for (auto& element : elements) window->draw(*element);
    FrameEnd();
}
