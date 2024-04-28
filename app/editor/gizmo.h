#pragma once

#include <algorithm>
#include <vector>

#include "data/gizmo.h"
#include "events/event.h"
#include "render/gizmo.h"
#include "render/model.h"
#include "stage/stage.h"

class Gizmo {
   public:
    enum Mode {
        Move = 0,
        Scale = 1,
        Rotate = 2,
    };

   public:
    Gizmo(events::Observer& observer);

    void Draw();
    void DrawPicking();

    void SetModel(render::Model* model);
    void SetMode(Mode mode);

 private:
	bool MoveEvent(sf::Event event, glm::vec3 move);
	bool ScaleEvent(sf::Event event, glm::vec3 move);

   private:
    std::unique_ptr<render::Gizmo> gizmos_[3]{
        data::loadGizmo("resources/gizmo/arrow.obj"),  // move
        data::loadGizmo("resources/gizmo/cube.obj"),   // scale
        data::loadGizmo("resources/gizmo/arrow.obj"),  // rotate
    };

    data::Shader gizmo_shader_;
    data::Shader gizmo_picking_;

    Mode current_mode_ = Mode::Move;
    render::Model* current_model_ = nullptr;

	std::vector<events::Event> hotkeys;
};
