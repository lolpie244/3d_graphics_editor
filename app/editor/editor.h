#include "editor/gizmo.h"

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

    std::shared_ptr<render::Model> model;

	Gizmo gizmo;
};
