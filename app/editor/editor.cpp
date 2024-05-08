#include "editor.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "data/texture.h"

void EditorStage::BindEvents() {
    opengl_context_->SetScaleMethod<events::DefaultScale>();
    opengl_context_->BindScale(observer_);

    events.push_back(observer_.KeyBind({sf::Keyboard::Escape}, [this](sf::Event event) {
        this->Stop(stage::StageState::Exit);
        return true;
    }));

    events.push_back(observer_.Bind(sf::Event::KeyPressed, [this](sf::Event event) {
        auto next = 0;
        if (event.key.code == sf::Keyboard::Left)
            next -= 1;

        if (event.key.code == sf::Keyboard::Right)
            next += 1;
        if (next)
            gizmo.SetModel(nullptr);

        current_draw_mode_ = (3 + current_draw_mode_ + next) % 3;
        return next;
    }));

    opengl_context_->BindPress(observer_, [&](sf::Event event) { return ContextPress(event); }, {sf::Mouse::Left});
    opengl_context_->BindDrag(observer_, [&](sf::Event event, glm::vec2 moved) { return CameraMove(event, moved); },
                              {sf::Mouse::Right, sf::Mouse::Middle});
    opengl_context_->BindScroll(observer_, [this](sf::Event event) { return CameraZoom(event); });

    for (auto& [_, model] : models) {
        model->BindPress(observer_, [&](sf::Event event) { return ModelPress(event, model.get()); }, {sf::Mouse::Left});
        model->BindDrag(observer_, [&](sf::Event event, glm::vec3 move) { return ModelDrag(event, move, model.get()); },
                        {sf::Mouse::Left});
    }
}

EditorStage::EditorStage() : gizmo(this->observer_) {
    auto theme = data::SvgTexture::loadFromFile("resources/theme.svg");
    opengl_context_->SetLeftCorner(50, 0);
    opengl_context_->Resize(glm::vec2(1700, 1080));

    camera_->Move(0.0f, 0.0f, 3.0f);
    camera_->SetOrigin(0, 0, 0);
    ///////////////////////////////////////////
    auto model = render::Model::loadFromFile("resources/cube.obj", render::MeshChange::Enable);

    model->Scale(0.5, 0.5, 0.5);
    model->texture = data::PngTexture::loadFromFile("resources/cube.png")->getTexture({0, 0});
    models[model->Id()] = std::move(model);
    ///////////////////////////////////////////
    BindEvents();
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
