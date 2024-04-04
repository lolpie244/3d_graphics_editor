#include "test_stage.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include "stage/stage_manager.h"
#include "test_stage1/test_stage1.h"

TestStage::TestStage() {
    events.push_back(observer_.Bind(sf::Event::KeyPressed, [this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->Stop(stage::StageState::Exit);
            return true;
        }
		if (event.key.code == sf::Keyboard::Enter) {
			stage::StageManager::Instance().NextStage(std::make_unique<TestStage1>());
			return true;
		}
        return false;
    }));
}


void TestStage::Run() {
	PollEvents();
	sf::CircleShape shape(50);

	shape.setFillColor(sf::Color(100, 250, 50));

	window->clear();
	window->draw(shape);
	FrameEnd();
}
