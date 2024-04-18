#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <future>
#include <memory>
#include <stack>

#include "render/camera.h"
#include "utils/singleton.h"
#include "utils/vector2.h"
#include "stage.h"

namespace stage {

class StageManager : public Singleton<StageManager> {
    friend Singleton<StageManager>;

   public:
    template <typename... _Args>
    void InitWindow(_Args&&... __args) {
        window_ = std::make_shared<sf::RenderWindow>(std::forward<_Args>(__args)...);
    }

   public:
    std::unique_ptr<Stage>& CurrentStage();
    void NextStage(std::unique_ptr<Stage> new_stage);
    void PreviousStage();

    void Start();
    void Exit();

    std::shared_ptr<sf::RenderWindow>& Window();
    std::unique_ptr<render::Camera>& Camera();
	utils::Vector2f windowSize();

   private:
    std::shared_ptr<sf::RenderWindow> window_;

    std::stack<std::unique_ptr<Stage>> stages_;
    bool exit_ = false;
};
}  // namespace stage
