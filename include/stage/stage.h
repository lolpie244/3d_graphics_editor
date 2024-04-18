#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <future>
#include <memory>

#include "events/event.h"
#include "events/observer.h"
#include "gui/elements/base.h"
#include "gui/elements_container.h"
#include "render/camera.h"
#include "utils/settings.h"

namespace stage {

enum StageState { Run, Pause, Exit };

class Stage {
   public:
	Stage();
    virtual ~Stage() = default;

    virtual void Run() = 0;
    virtual void Start();
    virtual void Stop(StageState with_state);

    virtual void PollEvents();
    virtual void FrameEnd();

    StageState State();
    std::unique_ptr<render::Camera>& Camera();

   protected:
    events::Observer observer_;
    std::atomic<StageState> state_ = StageState::Run;

    std::shared_ptr<sf::RenderWindow> window_;
    std::unique_ptr<render::Camera> camera_ = std::make_unique<render::Camera>(settings::DEFAULT_FOV);

    gui::ElementContainer elements_;
    std::vector<events::Event> events;
};
}  // namespace stage
