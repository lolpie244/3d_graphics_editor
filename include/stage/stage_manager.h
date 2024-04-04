#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <future>
#include <memory>
#include <stack>

#include "utils/singleton.h"

namespace stage {

class Stage;

class StageManager : public Singleton<StageManager> {
    friend Singleton<StageManager>;

   public:
    std::unique_ptr<Stage>& CurrentStage();
    void NextStage(std::unique_ptr<Stage> new_stage);
    void PreviousStage();

    void Start();

   public:
    std::shared_ptr<sf::RenderWindow> window;

   private:
    std::stack<std::unique_ptr<Stage>> stages_;
};
}  // namespace stage
