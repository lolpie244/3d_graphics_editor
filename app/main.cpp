#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <memory>

#include "editor/editor.h"
#include "menu/menu.h"
#include "render/camera.h"
#include "render/opengl/error_callback.h"
#include "stage/stage_manager.h"

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    stage::StageManager& stage_manager = stage::StageManager::Instance();
    stage_manager.InitWindow(sf::VideoMode(1920, 1080), "test", sf::Style::Fullscreen, settings);
    stage_manager.Window()->setFramerateLimit(160);
    stage_manager.Window()->setMouseCursorGrabbed(true);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebugMessageCallback, 0);
#endif

    auto test_stage = std::make_unique<MenuStage>();
    stage_manager.NextStage(std::move(test_stage));

    stage_manager.Start();
}
