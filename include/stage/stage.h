#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <future>

#include "events/observer.h"
#include "gui/elements/base.h"
#include "stage/stage_manager.h"
namespace stage {

enum StageState { Init, Run, Pause, Exit };

class Stage {
   public:
    virtual void Run() = 0;
	virtual void Init();
    virtual void Start();
    virtual void Stop(StageState with_state);

    virtual void PollEvents();
    virtual void FrameEnd();

	StageState State();

    virtual ~Stage() {}

   protected:
    events::Observer observer_;
    std::shared_ptr<sf::RenderWindow> window = StageManager::Instance().window;
    std::atomic<StageState> state_ = StageState::Init;

	std::vector<std::unique_ptr<gui::GuiElement>> elements;
	std::vector<std::unique_ptr<events::Event>> events;
};
}  // namespace stage
