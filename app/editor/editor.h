#include <SFML/Window/Event.hpp>

#include "editor/gizmo.h"
#include "editor/mode.h"
#include "events/event.h"
#include "render/model.h"
#include "render/opengl/picking_texture.h"

using render::PickingTexture;

class EditorStage : public stage::Stage {
	typedef std::unordered_set<PickingTexture::Info, PickingTexture::Info::Hash> SelectedVertices;

   public:
    EditorStage();
    void Run() override;
    void BindEvents();

    void ClearSelection();

   public:  // events
    bool CameraMove(sf::Event event, glm::vec2 moved);
    bool CameraZoom(sf::Event event);

    bool ContextPress(sf::Event event);
    bool ContextDrag(sf::Event event, glm::vec2 moved);
    bool ContextRelease(sf::Event event);

    bool ModelPress(sf::Event event, render::Model* model);
    bool ModelDrag(sf::Event event, glm::vec3 move, render::Model* model);

	bool DuplicateSelected(sf::Event event);
	bool JoinSelected(sf::Event event);

   private:
    std::vector<std::unique_ptr<gui::GuiElement>> elements;
    std::vector<events::Event> events;
    render::ModelsList models;

    Gizmo gizmo;
    std::unique_ptr<DrawMode> draw_modes_[3]{
        std::make_unique<TextureDraw>(),
        std::make_unique<MixedDraw>(),
        std::make_unique<TransparentDraw>(),
    };

    SelectedVertices selected_vertexes_;
    int current_draw_mode_ = 1;

	glm::vec3 last_vertex_position = {-1, -1, -1};
};
