#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <future>

#include "events/observer.h"
#include "stage/stage_manager.h"
namespace stage {

enum StageState { Run, Pause, Exit };

class Stage {
   public:
    virtual void Run() = 0;
    virtual void Start();
    virtual void Stop(StageState with_state);

    virtual void PollEvents();
    virtual void FrameEnd();

	StageState State();

    virtual ~Stage() {}

   protected:
    events::Observer observer_;
    std::shared_ptr<sf::RenderWindow> window = StageManager::Instance().window;
    std::atomic<StageState> state_ = StageState::Run;
};
}  // namespace stage
