#include "stage/stage_manager.h"

#include <future>
#include <iostream>
#include <memory>
#include <thread>

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
        if (CurrentStage()->State() == StageState::Run)
            CurrentStage()->Run();
        else
            PreviousStage();
    }
}
}  // namespace stage
