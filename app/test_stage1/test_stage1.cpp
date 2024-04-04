#include "test_stage1.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "stage/stage.h"

TestStage1::TestStage1() {
    events.push_back(observer_.Bind(sf::Event::KeyPressed, [this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->Stop(stage::StageState::Exit);
            return true;
        }
        return false;
    }));
}

void TestStage1::Run() {
	PollEvents();
	sf::CircleShape shape(50);

	shape.setFillColor(sf::Color(100, 250, 250));

	window->clear();
	window->draw(shape);
	FrameEnd();
}
