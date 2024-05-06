#include <GL/glew.h>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "data/model_loader.h"
#include "editor.h"
#include "math/points_cast.h"
#include "render/model.h"
#include "utils/settings.h"

bool EditorStage::CameraMove(sf::Event event, glm::vec2 moved) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        auto move = math::to_ndc(stage::StageManager::Instance().windowSize() / 2.0f + moved);
        stage::StageManager::Instance().Camera()->Move(-move.x, -move.y);
        return true;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
        stage::StageManager::Instance().Camera()->Rotate(moved.x * 0.5, math::Y);
        stage::StageManager::Instance().Camera()->Rotate(-moved.y * 0.5, math::X);
        return true;
    }
    return false;
}

bool EditorStage::CameraZoom(sf::Event event) {
    auto old_origin = Camera()->GetOrigin();
    Camera()->Move(0, 0, -event.mouseWheelScroll.delta);
    Camera()->SetOrigin(Camera()->GetOrigin().x, Camera()->GetOrigin().y, old_origin.z);
    return true;
}

void EditorStage::ClearSelection() {
    gizmo.SetModel(nullptr);
    for (auto& vertex : selected_vertexes_) {
        models[vertex.ObjectID]->SetVertexColor(vertex.VertexId, sf::Color::White);
    }
    selected_vertexes_.clear();
}

bool EditorStage::ContextPress(sf::Event event) {
    ClearSelection();
    return true;
}

bool EditorStage::ContextDrag(sf::Event event, glm::vec2 move) {
    ClearSelection();
    return true;
}

bool EditorStage::ContextRelease(sf::Event event) {
    ClearSelection();
    return true;
}

bool EditorStage::ModelPress(sf::Event event, render::Model* model) {
    last_vertex_position = {-1, -1, -1};

    if (model->PressInfo().Data == render::Model::Surface) {
        selected_vertexes_.clear();
        gizmo.SetModel(model);
    }
    if (model->PressInfo().Data == render::Model::Point) {
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !selected_vertexes_.contains(model->PressInfo()))
            ClearSelection();
        model->SetVertexColor(model->PressInfo().VertexId, sf::Color::Red);
        selected_vertexes_.insert(model->PressInfo());
    }
    return true;
}

bool EditorStage::ModelDrag(sf::Event event, glm::vec3 mouse_move, render::Model* model) {
    auto press_info = model->PressInfo();
    if (press_info.Data != render::Model::Point)
        return false;

    glm::vec3 origin = Camera()->GetRealPosition();
    glm::vec3 direction = glm::normalize(math::point_to_ray({event.mouseMove.x, event.mouseMove.y}));
    glm::vec3 vertex_position =
        model->GetTransformation() * glm::vec4(model->Vertex(press_info.VertexId).position, 1.0f);

    // TODO: fix point that is incorrect
    auto intersect_point = origin + direction * glm::dot(vertex_position - origin, direction);
    intersect_point = glm::inverse(model->GetTransformation()) * glm::vec4(intersect_point, 1.0f);

    if (this->last_vertex_position == glm::vec3(-1, -1, -1)) {
        this->last_vertex_position = intersect_point;
        return true;
    }
    auto move = intersect_point - last_vertex_position;
    model->SetVertexPosition(press_info.VertexId, intersect_point);

    for (auto& vertex_info : selected_vertexes_) {
        if (vertex_info == press_info)
            continue;

        auto* model = models[vertex_info.ObjectID].get();
        model->SetVertexPosition(vertex_info.VertexId, model->Vertex(vertex_info.VertexId).position + move);
    }

    this->last_vertex_position = intersect_point;
    return true;
}
