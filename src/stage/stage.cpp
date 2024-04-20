#include "stage/stage.h"

#include <SFML/Window/Event.hpp>

#include "events/propterties/scaleable.h"
#include "render/renderer.h"
#include "stage/stage_manager.h"

namespace stage {

Stage::Stage() : window_(stage::StageManager::Instance().Window()) {}

void Stage::Start() {
    state_ = StageState::Run;
    sf::Event event;
    event.type = sf::Event::Resized;
    event.size.width = StageManager::Instance().windowSize().x;
    event.size.height = StageManager::Instance().windowSize().y;
    observer_.Notify(event);
}

void Stage::Stop(StageState with_state) { state_ = with_state; }

void Stage::PollEvents() {
    sf::Event event;
    while (window_->pollEvent(event)) {
        observer_.Notify(event);

        if (event.type == sf::Event::Closed) {
            StageManager::Instance().Exit();
        }
        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window_->setView(sf::View(visibleArea));
        }
    }
}

void Stage::FrameEnd() {
    window_->pushGLStates();
    window_->draw(elements_);
    window_->popGLStates();

    window_->display();
    if (!window_->isOpen())
        this->Stop(StageState::Exit);
}

StageState Stage::State() { return state_; }
std::unique_ptr<render::Camera>& Stage::Camera() { return camera_; }
}  // namespace stage
