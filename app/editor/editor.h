#include <SFML/Window/Event.hpp>

#include "editor/gizmo.h"
#include "editor/mode.h"
#include "events/event.h"
#include "render/model.h"

class EditorStage : public stage::Stage {
   public:
    EditorStage();
    void Run() override;
    void BindEvents();

 public: // events
    bool CameraMove(sf::Event event, math::Vector2f moved);
    bool CameraZoom(sf::Event event);

    bool ContextPress(sf::Event event);
    bool ModelPress(sf::Event event, render::Model* model);
    bool ModelDrag(sf::Event event, glm::vec3 move, render::Model* model);

   public:
    std::vector<std::unique_ptr<gui::GuiElement>> elements;
    std::vector<events::Event> events;

    render::ModelsList models;

    Gizmo gizmo;
    std::unique_ptr<DrawMode> draw_modes_[3]{
        std::make_unique<TextureDraw>(),
        std::make_unique<MixedDraw>(),
        std::make_unique<TransparentDraw>(),
    };

    int current_draw_mode_ = 0;
};
