#include "gizmo.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <climits>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "editor/editor.h"
#include "editor/gizmo.h"
#include "math/ray.h"
#include "math/transform.h"

Gizmo::Gizmo(events::Observer& observer, float& scale): scale(scale) {
    gizmo_shader_.loadFromFile("shaders/gizmo.vert", "shaders/gizmo.frag");
    gizmo_picking_.loadFromFile("shaders/gizmo.vert", "shaders/picking.frag");

    for (auto& gizmo : gizmos_) {
        gizmo->Move(0, 0, 100);
    }
    BindEvents(observer);
}

glm::vec3 Gizmo::GetPoint(glm::vec2 mouse) {
    auto ray = math::Ray::FromPoint(mouse);
    auto gizmo_center = current_model_->GetPosition();
    auto axis = gizmos_[current_mode_]->PressInfo().Type;

    return ray.PlainIntersection(gizmo_center, math::axis_to_vector(axis));
}

bool Gizmo::PressEvent(sf::Event event) {
    old_point = GetPoint({event.mouseButton.x, event.mouseButton.y});
    return true;
}

bool Gizmo::MoveEvent(sf::Event event, glm::vec3 move) {
    move = scale * move * math::axis_to_vector(gizmos_[Mode::Move]->PressInfo().Type) * settings::MOUSE_SENSATIVITY;
    current_model_->Move(move.x, move.y, move.z);
    return true;
}
bool Gizmo::ScaleEvent(sf::Event event, glm::vec3 move) {
    move = scale * move * math::axis_to_vector(gizmos_[Mode::Scale]->PressInfo().Type) * settings::MOUSE_SENSATIVITY;
    auto scale = current_model_->GetScale() + move;
    current_model_->SetScale(scale.x, scale.y, scale.z);
    return true;
}

bool Gizmo::RotateEvent(sf::Event event, glm::vec3 move) {
    auto ray = math::Ray::FromPoint({event.mouseMove.x, event.mouseMove.y});
    auto gizmo_center = current_model_->GetPosition();

    auto axis = gizmos_[Mode::Rotate]->PressInfo().Type;

    glm::vec3 normal = current_model_->GetTransformation() * glm::vec4(math::axis_to_vector(axis), 1.0f);
    auto new_point = ray.PlainIntersection(gizmo_center, glm::normalize(normal));

    if (old_point == glm::vec3{-1, -1, -1}) {
        old_point = new_point;
        return true;
    }

    auto old_vec = (old_point - gizmo_center);
    auto new_vec = (new_point - gizmo_center);

    // TODO: fix
    float angle =
        glm::degrees(glm::orientedAngle(glm::normalize(old_vec), glm::normalize(new_vec), glm::normalize(normal)));

    current_model_->Rotate(angle, axis);
    gizmos_[Mode::Rotate]->Rotate(angle, axis);

    old_point = new_point;
    return true;
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

void Gizmo::SetModel(render::Model* model) {
    current_model_ = model;

	if (model == nullptr)
		return;

    auto [min, max] = model->ModelMesh().MeshBox();
    min = model->GetTransformation() * glm::vec4(min, 1.0f);
    max = model->GetTransformation() * glm::vec4(max, 1.0f);

    for (int gizmo_id = 0; gizmo_id < Mode::EnumSize; gizmo_id++) {
        float scale = INT_MAX;
        auto gizmo_box = gizmos_[gizmo_id]->ModelMesh().MeshBox();

        for (int i = 0; i < 3; i++)
            scale = std::min(scale, (max[i] - min[i]) / (gizmo_box.second[i] - gizmo_box.first[i]));

        gizmos_[gizmo_id]->SetScale(scale, scale, scale);
    }
}
void Gizmo::SetMode(Mode mode) { current_mode_ = mode; }

void Gizmo::BindEvents(events::Observer& observer) {
    gizmos_[Mode::Move]->BindDrag(observer, [this](sf::Event event, glm::vec3 move) { return MoveEvent(event, move); },
                                  {sf::Mouse::Left});

    gizmos_[Mode::Scale]->BindDrag(
        observer, [this](sf::Event event, glm::vec3 move) { return ScaleEvent(event, move); }, {sf::Mouse::Left});

    gizmos_[Mode::Rotate]->BindDrag(
        observer, [this](sf::Event event, glm::vec3 move) { return RotateEvent(event, move); }, {sf::Mouse::Left});

    std::pair<Gizmo::Mode, std::unordered_set<sf::Keyboard::Key>> mode_hotkeys[]{
        {Gizmo::Move, {sf::Keyboard::M}},
        {Gizmo::Scale, {sf::Keyboard::S}},
        {Gizmo::Rotate, {sf::Keyboard::R}},
    };

    for (auto& [mode, hotkey] : mode_hotkeys) {
        hotkeys.push_back(observer.KeyBind(hotkey, [this, mode](sf::Event event) {
            this->SetMode(mode);
            return true;
        }));
    }

    for (int i = 0; i < Mode::EnumSize; i++) {
        gizmos_[i]->BindPress(observer, [this](sf::Event event) { return PressEvent(event); });
    }
}
