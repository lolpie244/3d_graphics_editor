#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <memory>

#include "stage/stage_manager.h"
#include "test_stage/test_stage.h"
#include "test_stage1/test_stage1.h"
#include "render/opengl/error_callback.h"

int main() {
    stage::StageManager& stage_manager = stage::StageManager::Instance();
    stage_manager.window = std::make_shared<sf::RenderWindow>(sf::VideoMode(600, 600), "test");
    stage_manager.window->setFramerateLimit(60);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebugMessageCallback, 0);
#endif

    auto test_stage = std::make_unique<TestStage>();
    stage_manager.NextStage(std::move(test_stage));

    stage_manager.Start();
}
