#include "test_stage1.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

#include "gui/elements/buttons_list.h"
#include "stage/stage.h"
#include "utils/vector2.h"

TestStage1::TestStage1() {
    events.push_back(observer_.Bind(sf::Event::KeyPressed, [this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->Stop(stage::StageState::Exit);
            return true;
        }
        return false;
    }));

	// auto theme = utils::SvgTexture::loadFromFile("resources/theme1.svg");

}

void TestStage1::Run() {
	PollEvents();
	sf::CircleShape shape(50);


	window->clear();
	for (auto& element : elements) window->draw(*element);

	FrameEnd();
}
