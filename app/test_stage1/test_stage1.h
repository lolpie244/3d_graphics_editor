#include <SFML/Graphics/Shader.hpp>
#include <vector>
#include "events/event.h"
#include "render/mesh.h"
#include "stage/stage.h"
#include "gui/elements/base.h"
#include "render/shader.h"
#include "utils/vector2.h"

class TestStage1 : public stage::Stage {
   public:
	TestStage1();
    void Run() override;

	std::vector<std::unique_ptr<gui::GuiElement>> elements;
	std::vector<std::unique_ptr<events::mEventType>> events;

	render::Shader shader;
	sf::Texture texture;
	std::shared_ptr<render::Mesh> mesh1;
	std::shared_ptr<render::Mesh> mesh2;

	utils::Vector2f old_pos_ = {0, 0};
};
