#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <memory>
#include <vector>

#include "data/shader.h"
#include "gui/opengl_context.h"
#include "gui/sprite_element.h"
#include "render/gizmo.h"
#include "render/model.h"
#include "render/opengl/picking_texture.h"
#include "stage/stage.h"

class EditorStage : public stage::Stage {
   public:
    EditorStage();
    void Run() override;
	void InitGui();
	void InitScene();



    std::vector<std::unique_ptr<gui::GuiElement>> elements;
    std::vector<std::unique_ptr<events::mEventType>> events;

    data::Shader shader;
    data::Shader point_shader;
    data::Shader picking_shader;
    data::Shader gizmo_shader;
    data::Shader gizmo_picking;

    std::shared_ptr<render::Model> model;
    std::shared_ptr<render::Gizmo> gizmo;

    render::PickingTexture::Info pixel;
	
    gui::OpenglContext opengl_context_;
};
