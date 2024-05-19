#pragma once

#include <algorithm>
#include <vector>

#include "events/event.h"
#include "events/observer.h"
#include "render/gizmo.h"
#include "render/model.h"
#include "stage/stage.h"

class Gizmo {
   public:
    enum Mode {
        Move,
        Scale,
        Rotate,
        EnumSize,
    };

   public:
    Gizmo(events::Observer& observer, float& scale);

    void Draw();
    void DrawPicking();

    void SetModel(render::Model* model);
    void SetMode(Mode mode);

   private:
    void BindEvents(events::Observer& observer);

    bool PressEvent(sf::Event event);
    bool MoveEvent(sf::Event event, glm::vec3 move);
    bool ScaleEvent(sf::Event event, glm::vec3 move);
    bool RotateEvent(sf::Event event, glm::vec3 move);

    render::Gizmo* CurrentGizmo() { return gizmos_[current_mode_].get(); }
	
 private:
	glm::vec3 GetPoint(glm::vec2 mouse);

   private:
    std::unique_ptr<render::Gizmo> gizmos_[3]{
        render::Gizmo::loadFromFile("resources/gizmo/arrow.obj"),   // move
        render::Gizmo::loadFromFile("resources/gizmo/cube.obj"),    // scale
        render::Gizmo::loadFromFile("resources/gizmo/circle.obj"),  // rotate
    };

	const float& scale;

    data::Shader gizmo_shader_;
    data::Shader gizmo_picking_;

    Mode current_mode_ = Mode::Move;
    render::Model* current_model_ = nullptr;
    glm::vec3 old_point = {-1, -1, -1};

    std::vector<events::Event> hotkeys;
};
