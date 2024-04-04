#include "test_stage.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

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
}

void TestStage::Run() {
    PollEvents();
    sf::CircleShape shape(50);

    shape.setFillColor(sf::Color(100, 250, 50));

    window->clear();
    window->draw(shape);
    FrameEnd();
}
