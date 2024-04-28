#include "editor.h"

#include <GL/glew.h>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "data/model_loader.h"
#include "data/texture.h"
#include "math/points_cast.h"
#include "render/model.h"
#include "utils/settings.h"

EditorStage::EditorStage() : gizmo(this->observer_) {
    events.push_back(observer_.Bind(sf::Event::KeyPressed, [this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->Stop(stage::StageState::Exit);
            return true;
        }
        return false;
    }));

    auto theme = data::SvgTexture::loadFromFile("resources/theme.svg");
    opengl_context_->SetLeftCorner(50, 0);
    opengl_context_->Resize(math::Vector2f(1700, 1080));
    opengl_context_->SetScaleMethod<events::DefaultScale>();
    opengl_context_->BindScale(observer_);

    opengl_context_->BindPress(observer_, [this](sf::Event event) {
        if (event.mouseButton.button != sf::Mouse::Left)
            return false;
        gizmo.SetModel(nullptr);
        return true;
    });

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
    auto model = data::loadModel("resources/cube.obj", render::MeshChange::Enable);

    model->Scale(0.5, 0.5, 0.5);
    model->texture = data::PngTexture::loadFromFile("resources/cube.png")->getTexture({0, 0});
    ///////////////////////////////////////////

    models.push_back(std::move(model));

    for (auto& model : models) {
        model->BindPress(observer_,
                         [this, &model](sf::Event event) {
                             std::cout << model->PressInfo().Data << '\n';

                             if (model->PressInfo().Data == render::Model::Surface)
                                 gizmo.SetModel(model.get());
                             return true;
                         },
                         {sf::Mouse::Left});

        model->BindDrag(observer_, [this, &model](sf::Event event, glm::vec3 move) {
            if (model->PressInfo().Data != render::Model::Point)
                return false;
            model->SetVertexPosition(model->PressInfo().VertexId, model->Vertex(model->PressInfo().VertexId).position +
                                                                      move * settings::MOUSE_SENSATIVITY);
            return true;
        });
    }
}

void EditorStage::Run() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        opengl_context_->PickingTexture.Bind();
        draw_modes_[current_draw_mode_]->DrawPicker(models);
        gizmo.DrawPicking();
        opengl_context_->PickingTexture.Unbind();
    }
    PollEvents();
    draw_modes_[current_draw_mode_]->Draw(models);
    gizmo.Draw();
    FrameEnd();
}
