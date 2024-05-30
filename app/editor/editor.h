#include <SFML/Window/Event.hpp>

#include "editor/mode.h"
#include "editor/network/network.h"
#include "events/event.h"
#include "gui/select_rect.h"
#include "network/communication_socket.h"
#include "render/gizmo.h"
#include "render/light.h"
#include "render/model.h"
#include "render/opengl/picking_texture.h"
#include "utils/utils.h"

using render::PickingTexture;

class EditorStage : public stage::Stage {
    using SelectedVertices = render::Model::SelectedVertices;

   public:
    EditorStage();
    void Run() override;
    void InitGui();
    void BindEvents();
    void AddModel(std::unique_ptr<render::Model> model);
    void AddModelFromFile(const std::string& filename);
    void AddModelFromMemory(int id, const tcp_socket::BytesType& filename);
    void AddLight(glm::vec4 color);

    void Select(render::PickingTexture::Info info);
    void ClearSelection();

    float Scale() const { return std::max(0.05f, scale_); }

    void PerformPendingFunctions();
	bool MoveSelectedPoints(sf::Event event, render::PickingTexture::Info press_info);

	void SendUpdate(std::function<void(void)>);

   public:  // events
    bool CameraMove(sf::Event event, glm::vec2 moved);
    bool CameraZoom(sf::Event event);

    bool ContextPress(sf::Event event);
    bool ContextDrag(sf::Event event, glm::vec2 moved);
    bool ContextRelease(sf::Event event);

    bool ModelPress(sf::Event event, render::Model* model);
    bool ModelRelease(sf::Event event, render::Model* model);

	bool DeleteModel(sf::Event event);

    bool LightPress(sf::Event event, render::Light* light);

    bool DuplicateSelected(sf::Event event);
    bool JoinSelected(sf::Event event);

	bool SaveAsScene();
	bool OpenScene();
	bool ImportModel();

   public:
	std::list<std::function<void()>> PendingFunctions;
    render::ModelsList models;
   private:
    render::LightList lights;
    std::vector<events::Event> events;

    std::pair<sf::String, std::unique_ptr<DrawMode>> draw_modes_[3]{
        {L"", std::make_unique<TextureDraw>()},
        {L"󱎖", std::make_unique<MixedDraw>()},
        {L"", std::make_unique<TransparentDraw>()},
    };

    std::pair<sf::String, std::unique_ptr<render::Gizmo>> gizmos_[3]{
        {L"", render::Gizmo::loadFromFile<render::TranslateGizmo>("resources/gizmo/arrow.obj")},
        {L"󰩨", render::Gizmo::loadFromFile<render::ScaleGizmo>("resources/gizmo/cube.obj")},
        {L"󰶘", render::Gizmo::loadFromFile<render::RotateGizmo>("resources/gizmo/circle.obj")},
    };

    std::pair<sf::String, sf::String> default_figures_[4]{
        {L"Куб", "resources/default/cube.obj"},
        {L"Площина", "resources/default/plane.obj"},
        {L"Піраміда", "resources/default/pyramid.obj"},
        {L"С'юзан", "resources/default/susan.obj"},
    };

    DrawMode* current_draw_mode_ = draw_modes_[0].second.get();
    render::Gizmo* current_gizmo_ = gizmos_[0].second.get();

    data::Shader gizmo_shader_;
    data::Shader gizmo_picking_;

    std::shared_ptr<gui::SelectRect> selection_rect_;

    SelectedVertices selected_vertexes_;

    glm::vec3 last_vertex_position = {-1, -1, -1};
    float scale_ = 1;

    std::unique_ptr<Collaborator> connection_;
	std::list<std::future<void>> requests_;

	bool pending_move_ = false;
};
