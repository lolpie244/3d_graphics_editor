#include <SFML/Graphics/Shader.hpp>
#include <vector>
#include "events/event.h"
#include "render/mesh.h"
#include "stage/stage.h"
#include "data/shader.h"
#include "math/vector2.h"

class TestStage1 : public stage::Stage {
   public:
	TestStage1();
    void Run() override;

	std::vector<std::unique_ptr<gui::GuiElement>> elements;
	std::vector<std::unique_ptr<events::mEventType>> events;

	data::Shader shader;
	sf::Texture texture;
	std::shared_ptr<render::Mesh> mesh1;
	std::shared_ptr<render::Mesh> mesh2;

	math::Vector2f old_pos_ = {0, 0};
};
