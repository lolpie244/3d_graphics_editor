#include <SFML/Graphics/Shader.hpp>
#include <vector>
#include "events/event.h"
#include "gui/opengl_context.h"
#include "gui/sprite_element.h"
#include "render/mesh.h"
#include "render/opengl/picking_texture.h"
#include "stage/stage.h"
#include "data/shader.h"
#include "math/vector2.h"

class TestStage1 : public stage::Stage {
   public:
	TestStage1();
    void Run() override;

	std::unique_ptr<gui::SpriteGuiElement> background;
	std::unique_ptr<gui::OpenglContext> opengl_context;

	std::vector<std::unique_ptr<gui::GuiElement>> elements;
	std::vector<std::unique_ptr<events::mEventType>> events;

	data::Shader shader;
	data::Shader point_shader;
	data::Shader picking_shader;

	sf::Texture texture;
	std::shared_ptr<render::Mesh> mesh;

	render::PickingTexture picking = render::PickingTexture(800, 800);
};
