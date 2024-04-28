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
        stage::StageManager::Instance().Camera()->Move(-move.x, move.y);
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
    stage::StageManager::Instance().Camera()->Move(0, 0, -event.mouseWheelScroll.delta);
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
    if (event.mouseButton.button != sf::Mouse::Left)
        return false;

    ClearSelection();
    return true;
}

bool EditorStage::ContextRelease(sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        // for (float x = std::min(b))
    }

    ClearSelection();
    return true;
}

bool EditorStage::ModelPress(sf::Event event, render::Model* model) {
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

bool EditorStage::ModelDrag(sf::Event event, glm::vec3 move, render::Model* model) {
    auto press_info = model->StartPointInfo();
    if (press_info.Data != render::Model::Point)
        return false;
    auto update_pos = [move](auto model, auto info) {
        model->SetVertexPosition(info.VertexId,
                                 model->Vertex(info.VertexId).position + move * settings::MOUSE_SENSATIVITY);
    };

    for (auto& vertex : selected_vertexes_) update_pos(models[vertex.ObjectID].get(), vertex);

    return true;
}
