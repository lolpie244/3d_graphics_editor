#include "stage/stage_manager.h"

#include <future>
#include <iostream>
#include <memory>
#include <thread>

#include "renderer/renderer.h"
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
		renderer::GL_Renderer::Instance().Clear();

        if (exit_) {
            CurrentStage()->Stop(StageState::Exit);
            PreviousStage();
            continue;
        }

        if (CurrentStage()->State() == StageState::Run) {
            CurrentStage()->Run();
			CurrentStage()->FrameEnd();
		}
        else
            PreviousStage();
    }
}
void StageManager::Exit() { exit_ = true; }
}  // namespace stage
