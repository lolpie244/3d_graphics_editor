#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <future>
#include <memory>
#include <stack>

#include "gui/opengl_context.h"
#include "render/camera.h"
#include "stage.h"
#include "utils/singleton.h"

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

    std::unique_ptr<render::Camera>& Camera();
    std::shared_ptr<sf::RenderWindow>& Window();
	std::unique_ptr<gui::OpenglContext>& Context();
    math::Vector2f windowSize();

   private:
    std::shared_ptr<sf::RenderWindow> window_;

    std::stack<std::unique_ptr<Stage>> stages_;
    bool exit_ = false;
};
}  // namespace stage
