#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <memory>
#include "stage/stage_manager.h"
#include "test_stage/test_stage.h"

int main() {
	stage::StageManager& stage_manager = stage::StageManager::Instance();
	stage_manager.window = std::make_shared<sf::RenderWindow>(sf::VideoMode(600, 360), "test");
	stage_manager.window->setFramerateLimit(60);

	auto test_stage = std::make_unique<TestStage>();
	stage_manager.NextStage(std::move(test_stage));

	stage_manager.Start();
	// stage_manager.Wait();
}
