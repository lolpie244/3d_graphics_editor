#include <SFML/Graphics/Shader.hpp>
#include <vector>
#include "events/event.h"
#include "render/mesh.h"
#include "stage/stage.h"
#include "gui/elements/base.h"
#include "utils/shader.h"

class TestStage1 : public stage::Stage {
   public:
	TestStage1();
    void Run() override;

	std::vector<std::unique_ptr<gui::GuiElement>> elements;
	std::vector<std::unique_ptr<events::mEventType>> events;

	utils::Shader shader;
	sf::Texture texture;
	std::shared_ptr<render::Mesh> mesh;
};
