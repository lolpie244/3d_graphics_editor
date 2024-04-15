#include "stage/stage.h"

#include <SFML/Window/Event.hpp>

#include "gui/propteries/scaleable.h"
#include "stage/stage_manager.h"

namespace stage {

void Stage::Start() {
    state_ = StageState::Run;
}

void Stage::Stop(StageState with_state) { state_ = with_state; }

void Stage::PollEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        observer_.Notify(event);

        if (event.type == sf::Event::Closed) {
            StageManager::Instance().Exit();
        }
        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window->setView(sf::View(visibleArea));
        }
    }
}

void Stage::FrameEnd() {
    window->clear();
	window->draw(elements_);
    window->display();

    if (!window->isOpen())
        this->Stop(StageState::Exit);
}

StageState Stage::State() { return state_; }
}  // namespace stage
