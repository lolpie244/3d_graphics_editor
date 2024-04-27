#include "editor.h"

#include <GL/glew.h>

#include "data/model_loader.h"
#include "math/points_cast.h"

EditorStage::EditorStage() {
    events.push_back(observer_.Bind(sf::Event::KeyPressed, [this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->Stop(stage::StageState::Exit);
            return true;
        }
        return false;
    }));

    auto theme = data::SvgTexture::loadFromFile("resources/theme.svg");
    shader.loadFromFile("shaders/texture.vert", "shaders/texture.frag");
    point_shader.loadFromFile("shaders/texture.vert", "shaders/point.frag");
    picking_shader.loadFromFile("shaders/texture.vert", "shaders/picking.frag");
    gizmo_shader.loadFromFile("shaders/gizmo.vert", "shaders/gizmo.frag");
    gizmo_picking.loadFromFile("shaders/gizmo.vert", "shaders/picking.frag");

    opengl_context_->SetLeftCorner(50, 0);
    opengl_context_->Resize(math::Vector2f(1700, 1080));
    opengl_context_->SetScaleMethod<events::DefaultScale>();
    opengl_context_->BindScale(observer_);

    opengl_context_->BindDrag(observer_, [this](sf::Event event, math::Vector2f moved) {
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
    });

    opengl_context_->BindScroll(observer_, [this](sf::Event event) {
        stage::StageManager::Instance().Camera()->Move(0, 0, -event.mouseWheelScroll.delta);
        return true;
    });

    camera_->Move(0, 0, 3.0f);
    camera_->Rotate(-40, math::X);
    ///////////////////////////////////////////
    model = data::loadModel("resources/cube.obj", render::MeshChange::Enable);

    model->Scale(0.5, 0.5, 0.5);
    model->texture = data::PngTexture::loadFromFile("resources/cube.png")->getTexture({0, 0});
    ///////////////////////////////////////////
    gizmo = data::loadGizmo("resources/gizmo/arrow.obj");

    model->BindDrag(observer_, [this](sf::Event event, glm::vec3 move) {
        model->SetVertexPosition(model->PressInfo().VertexId,
                                 model->Vertex(model->PressInfo().VertexId).position + move * 5.0f);
        return true;
    });
}

void EditorStage::Run() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        opengl_context_->PickingTexture.Bind();
        model->DrawPoints(picking_shader);
        // gizmo->Draw(gizmo_picking, model.get());
        opengl_context_->PickingTexture.Unbind();
    }
    PollEvents();
    model->Draw(shader);
    gizmo->Draw(gizmo_shader, model.get());
    FrameEnd();
}
