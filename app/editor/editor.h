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

    struct SceneData {
        std::vector<tcp_socket::BytesType> models;
        std::vector<tcp_socket::BytesType> ligths;
    };

   public:
    constexpr static render::MeshConfig DEFAULT_MODEL_CONFIG{.changeable = render::MeshConfig::Dynamic,
                                                             .triangulate = true};

    constexpr static render::Light::LightData DEFAULT_LIGHT_DATA{
        .ambient = {0.5, 0.5, 0.5}, .diffuse = {0.5, 0.5, 0.5}, .specular = {0.5, 0.5, 0.5}};

   public:
    EditorStage();
    void Run() override;
    void Clear();

    void InitGui();
    void BindEvents();

    void ScheduleWork(std::function<void(void)>);
    void RunAsync(std::function<void(void)>);
    void SendRequest(std::function<void(Collaborator*)>);

    void PerformPendingFunctions();
    float Scale() const;

    void AddModel(std::unique_ptr<render::Model> model, bool send_request = true);
    void AddLight(std::unique_ptr<render::Light> light, bool send_request = true);

    void SetFilename(const char* filename);

    void LoadScene(const tcp_socket::BytesType& data);
    tcp_socket::BytesType SceneToBytes();

    void Select(render::PickingTexture::Info info);
    void ClearSelection();

    bool MoveSelectedPoints(sf::Event event, render::PickingTexture::Info press_info);

	bool ClientButton(sf::Event event);
	bool ServerButton(sf::Event event);

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

    bool AddLight(sf::Event event);

    bool NewScene();
    bool SaveScene();
    bool SaveAsScene();
    bool OpenScene();
    bool ImportModel();

   public:
    render::ModelsList models;
    render::LightList lights;

   private:
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
	std::function<void(Collaborator*)> after_gizmo_transform_;

    std::shared_ptr<gui::SelectRect> selection_rect_;

    SelectedVertices selected_vertexes_;

    glm::vec3 last_vertex_position = {-1, -1, -1};

    std::unique_ptr<Collaborator> connection_;
    std::list<std::future<void>> requests_;
    std::list<std::function<void()>> pending_functions_;

    bool pending_move_ = false;

    const char* current_filename_;
};
