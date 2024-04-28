#include "gizmo.h"

#include <SFML/Window/Mouse.hpp>

#include "data/gizmo.h"
#include "editor/editor.h"
#include "editor/gizmo.h"
#include "math/transform.h"

Gizmo::Gizmo(events::Observer& observer) {
    gizmo_shader_.loadFromFile("shaders/gizmo.vert", "shaders/gizmo.frag");
    gizmo_picking_.loadFromFile("shaders/gizmo.vert", "shaders/picking.frag");

    for (auto& gizmo : gizmos_) {
        gizmo->SetScale(0.5, 0.5, 0.5);
        gizmo->Move(0, 0, 100);
    }

    gizmos_[0]->BindDrag(observer,
                         [this](sf::Event event, glm::vec3 move) {
                             move = move * math::axis_to_vector(gizmos_[0]->PressInfo().Data) *
                                    settings::MOUSE_SENSATIVITY;
                             current_model_->Move(move.x, move.y, move.z);
                             return true;
                         },
                         {sf::Mouse::Left});

    gizmos_[1]->BindDrag(observer,
                         [this](sf::Event event, glm::vec3 move) {
                             move = move * math::axis_to_vector(gizmos_[1]->PressInfo().Data) *
                                    settings::MOUSE_SENSATIVITY;
                             auto scale = current_model_->GetScale() + move;
                             current_model_->SetScale(scale.x, scale.y, scale.z);
                             return true;
                         },
                         {sf::Mouse::Left});

    hotkeys.push_back(observer.KeyBind({sf::Keyboard::M}, [this](sf::Event event) {
        this->SetMode(Gizmo::Move);
        return true;
    }));

    hotkeys.push_back(observer.KeyBind({sf::Keyboard::S}, [this](sf::Event event) {
        this->SetMode(Gizmo::Scale);
        return true;
    }));
}

void Gizmo::Draw() {
    if (!current_model_)
        return;
    gizmos_[current_mode_]->Draw(gizmo_shader_, current_model_);
}

void Gizmo::DrawPicking() {
    if (!current_model_)
        return;

    gizmos_[current_mode_]->Draw(gizmo_picking_, current_model_);
}

void Gizmo::SetModel(render::Model* model) { current_model_ = model; }
void Gizmo::SetMode(Mode mode) { current_mode_ = mode; }
