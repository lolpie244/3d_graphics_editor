#include "stage/stage_manager.h"

#include <cassert>
#include <future>
#include <iostream>
#include <memory>
#include <thread>

#include "render/renderer.h"
#include "stage/stage.h"

namespace stage {
std::unique_ptr<Stage>& StageManager::CurrentStage() { return stages_.top(); }

void StageManager::NextStage(std::unique_ptr<Stage> new_stage) {
    if (!stages_.empty())
        CurrentStage()->Stop(StageState::Pause);
    stages_.push(std::move(new_stage));
    CurrentStage()->Start();
}

void StageManager::PreviousStage() {
    stages_.pop();
    if (!stages_.empty())
        CurrentStage()->Start();
}

void StageManager::Start() {
    while (!stages_.empty()) {
        render::GL_render::Instance().Clear();

        if (exit_) {
            CurrentStage()->Stop(StageState::Exit);
            PreviousStage();
            continue;
        }

        if (CurrentStage()->State() == StageState::Run) {
            CurrentStage()->Run();
            CurrentStage()->FrameEnd();
        } else
            PreviousStage();
    }
}

std::shared_ptr<sf::RenderWindow>& StageManager::Window() {
    assert(window_ != nullptr && "Window is not initialized");
    return window_;
}

std::unique_ptr<render::Camera>& StageManager::Camera() {
    assert(!stages_.empty() && "No stage (with camera)");
    return CurrentStage()->Camera();
}

math::Vector2f StageManager::windowSize() { return math::Vector2f(Window()->getSize().x, Window()->getSize().y); }

void StageManager::Exit() { exit_ = true; }
}  // namespace stage
