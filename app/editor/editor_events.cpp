#include <GL/glew.h>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iterator>

#include "data/model_loader.h"
#include "editor.h"
#include "math/points_cast.h"
#include "math/ray.h"
#include "math/utils.h"
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
        models[vertex.ObjectID]->SetVertexColor(vertex.VertexId, vertex.Type, settings::DEFAULT_POINT_COLOR);
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

    if (model->PressInfo().Type == render::Model::Surface) {
        selected_vertexes_.clear();
        gizmo.SetModel(model);
    }
    if (model->PressInfo().Type == render::Model::Point || model->PressInfo().Type == render::Model::Pending) {
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !selected_vertexes_.contains(model->PressInfo()))
            ClearSelection();
        model->SetVertexColor(model->PressInfo().VertexId, model->PressInfo().Type, settings::SELECTED_POINT_COLOR);
        selected_vertexes_.insert(model->PressInfo());
    }
    return true;
}

bool EditorStage::ModelDrag(sf::Event event, glm::vec3 mouse_move, render::Model* model) {
    auto press_info = model->PressInfo();
    if (press_info.Type != render::Model::Point && press_info.Type != render::Model::Pending)
        return false;

    math::Ray ray = math::Ray::FromPoint({event.mouseMove.x, event.mouseMove.y});
    render::ModelVertex vertex;

    glm::vec3 vertex_position =
        model->GetTransformation() * glm::vec4(model->Vertex(press_info.VertexId, press_info.Type).position, 1.0f);

    // TODO: fix point that is incorrect
    auto intersect_point = ray.SphereIntersection(vertex_position);
    intersect_point = glm::inverse(model->GetTransformation()) * glm::vec4(intersect_point, 1.0f);

    if (this->last_vertex_position == glm::vec3(-1, -1, -1)) {
        this->last_vertex_position = intersect_point;
        return true;
    }
    auto move = intersect_point - last_vertex_position;
    model->SetVertexPosition(press_info.VertexId, press_info.Type, intersect_point);

    for (auto& vertex_info : selected_vertexes_) {
        if (vertex_info == press_info)
            continue;

        auto* model = models[vertex_info.ObjectID].get();
        model->SetVertexPosition(vertex_info.VertexId, vertex_info.Type,
                                 model->Vertex(vertex_info.VertexId, vertex_info.Type).position + move);
    }

    this->last_vertex_position = intersect_point;
    return true;
}

bool EditorStage::DuplicateSelected(sf::Event event) {
    SelectedVertices new_selected;
    for (auto vertex : selected_vertexes_) {
        vertex.VertexId =
            models[vertex.ObjectID]->AddPenging(models[vertex.ObjectID]->Vertex(vertex.VertexId, vertex.Type));
        vertex.Type = render::Model::Pending;
        new_selected.insert(vertex);
        models[vertex.ObjectID]->SetVertexColor(vertex.VertexId, vertex.Type, settings::SELECTED_POINT_COLOR);
    }

    ClearSelection();
    selected_vertexes_ = new_selected;
    return true;
}

bool EditorStage::JoinSelected(sf::Event event) {
    if (selected_vertexes_.empty())
        return false;

    auto model = models[selected_vertexes_.begin()->ObjectID].get();

    std::vector<unsigned int> indices;
    std::vector<unsigned int> pending_vertices;

    for (auto vertex : selected_vertexes_) {
        if (vertex.Type == render::Model::Point)
            indices.push_back(vertex.VertexId);
        else {
            model->SetVertexColor(vertex.VertexId, vertex.Type, settings::DEFAULT_POINT_COLOR);
            pending_vertices.push_back(vertex.VertexId);
        }
    }

    auto new_indices = model->RemovePendings(pending_vertices);
    std::move(new_indices.begin(), new_indices.end(), std::back_inserter(indices));

    math::sort_clockwise_polygon(model->Vertices(render::Model::Point), indices);

    model->AddFace(indices);
    return true;
}
