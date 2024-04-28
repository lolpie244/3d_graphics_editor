#include "editor.h"

#include <GL/glew.h>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "data/model_loader.h"
#include "math/points_cast.h"
#include "render/model.h"
#include "utils/settings.h"

bool EditorStage::CameraMove(sf::Event event, math::Vector2f moved) {
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

bool EditorStage::ContextPress(sf::Event event) {
    if (event.mouseButton.button != sf::Mouse::Left)
        return false;
    gizmo.SetModel(nullptr);
    return true;
}

bool EditorStage::ModelPress(sf::Event event, render::Model* model) {
    if (model->PressInfo().Data == render::Model::Surface)
        gizmo.SetModel(model);
    return true;
}

bool EditorStage::ModelDrag(sf::Event event, glm::vec3 move, render::Model* model) {
    auto press_info = model->PressInfo();
    if (press_info.Data != render::Model::Point)
        return false;

    model->SetVertexPosition(press_info.VertexId,
                             model->Vertex(press_info.VertexId).position + move * settings::MOUSE_SENSATIVITY);
    return true;
}

