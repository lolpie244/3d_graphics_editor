#include "editor/gizmo.h"
#include "editor/mode.h"
#include "render/model.h"

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

    render::ModelsList models;

    Gizmo gizmo;
    std::unique_ptr<DrawMode> draw_modes_[3]{
        std::make_unique<TextureDraw>(),
        std::make_unique<MixedDraw>(),
        std::make_unique<TransparentDraw>(),
    };

    int current_draw_mode_ = 0;
};
