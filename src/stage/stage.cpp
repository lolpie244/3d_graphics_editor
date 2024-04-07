#include "stage/stage.h"
#include <SFML/Window/Event.hpp>
#include "stage/stage_manager.h"

namespace stage {

void Stage::Start() {
	state_ = StageState::Run;
}

void Stage::Stop(StageState with_state) {
	state_ = with_state;
}

void Stage::PollEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
		observer_.Notify(event);

		if (event.type == sf::Event::Closed) {
			StageManager::Instance().Exit();
		}
	}
}

void Stage::FrameEnd() {
    window->display();

	if (!window->isOpen())
		this->Stop(StageState::Exit);
}

StageState Stage::State() {
	return state_;
}
}  // namespace stage
