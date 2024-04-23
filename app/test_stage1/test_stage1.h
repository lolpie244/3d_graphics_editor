#include <SFML/Graphics/Shader.hpp>
#include <vector>

#include "data/shader.h"
#include "gui/opengl_context.h"
#include "gui/sprite_element.h"
#include "render/model.h"
#include "render/opengl/picking_texture.h"
#include "stage/stage.h"

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
    std::shared_ptr<render::Model> model;

    render::PickingTexture picking = render::PickingTexture(800, 800);
    render::PickingTexture::Info pixel;
};
