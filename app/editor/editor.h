#include <SFML/Window/Event.hpp>

#include "editor/gizmo.h"
#include "editor/mode.h"
#include "events/event.h"
#include "gui/select_rect.h"
#include "render/model.h"
#include "render/opengl/picking_texture.h"

using render::PickingTexture;

class EditorStage : public stage::Stage {
	using SelectedVertices = render::Model::SelectedVertices;

   public:
    EditorStage();
    void Run() override;
    void BindEvents();

	void Select(render::PickingTexture::Info info);
    void ClearSelection();

   public:  // events
    bool CameraMove(sf::Event event, glm::vec2 moved);
    bool CameraZoom(sf::Event event);

    bool ContextPress(sf::Event event);
    bool ContextDrag(sf::Event event, glm::vec2 moved);
    bool ContextRelease(sf::Event event);

    bool ModelPress(sf::Event event, render::Model* model);
    bool ModelDrag(sf::Event event, glm::vec3 move, render::Model* model);
    bool ModelRelease(sf::Event event, render::Model* model);

	bool DuplicateSelected(sf::Event event);
	bool JoinSelected(sf::Event event);

   private:
    std::vector<events::Event> events;
    render::ModelsList models;

    Gizmo gizmo;
    std::unique_ptr<DrawMode> draw_modes_[3]{
        std::make_unique<TextureDraw>(),
        std::make_unique<MixedDraw>(),
        std::make_unique<TransparentDraw>(),
    };
	std::shared_ptr<gui::SelectRect> selection_rect_;

    SelectedVertices selected_vertexes_;
    int current_draw_mode_ = 0;

	glm::vec3 last_vertex_position = {-1, -1, -1};
	float scale = 1;
};
