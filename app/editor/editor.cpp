#include "editor.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <memory>

#include "data/texture.h"
#include "editor/network/network.h"
#include "gui/select_rect.h"
#include "render/mesh.h"

void EditorStage::BindEvents() {
    opengl_context_->SetScaleMethod<events::DefaultScale>();
    opengl_context_->BindScale(observer_);

    events.push_back(observer_.KeyBind({sf::Keyboard::Escape}, [this](sf::Event event) {
        this->Stop(stage::StageState::Exit);
        return true;
    }));

    events.push_back(observer_.KeyBind({sf::Keyboard::LControl, sf::Keyboard::D},
                                       [this](sf::Event event) { return DuplicateSelected(event); }));

    events.push_back(observer_.KeyBind({sf::Keyboard::LControl, sf::Keyboard::J},
                                       [this](sf::Event event) { return JoinSelected(event); }));

    events.push_back(observer_.KeyBind({sf::Keyboard::LControl, sf::Keyboard::H}, [this](sf::Event event) {
        if (connection_ == nullptr)
            connection_ = std::make_unique<Host>(this);
        return true;
    }));

    events.push_back(observer_.KeyBind({sf::Keyboard::LControl, sf::Keyboard::C}, [this](sf::Event event) {
        if (connection_ == nullptr)
            connection_ = std::make_unique<Client>(this);
        return true;
    }));

    events.push_back(observer_.KeyBind({sf::Keyboard::LControl, sf::Keyboard::J},
                                       [this](sf::Event event) { return JoinSelected(event); }));

    opengl_context_->BindPress(observer_, [&](sf::Event event) { return ContextPress(event); }, {sf::Mouse::Left});
    opengl_context_->BindRelease(observer_, [&](sf::Event event) { return ContextRelease(event); }, {sf::Mouse::Left});
    opengl_context_->BindDrag(observer_, [&](sf::Event event, glm::vec2 moved) { return ContextDrag(event, moved); },
                              {sf::Mouse::Left});
    opengl_context_->BindDrag(observer_, [&](sf::Event event, glm::vec2 moved) { return CameraMove(event, moved); },
                              {sf::Mouse::Right, sf::Mouse::Middle});
    opengl_context_->BindScroll(observer_, [this](sf::Event event) { return CameraZoom(event); });

    for (auto& [_, model] : models) {
        model->BindPress(observer_, [&](sf::Event event) { return ModelPress(event, model.get()); }, {sf::Mouse::Left});
        model->BindRelease(observer_, [&](sf::Event event) { return ModelRelease(event, model.get()); },
                           {sf::Mouse::Left});
        model->BindDrag(observer_, [&](sf::Event event, glm::vec3 move) { return ModelDrag(event, move, model.get()); },
                        {sf::Mouse::Left});
    }
}

EditorStage::EditorStage() : gizmo(this->observer_, this) {
    camera_->Move(0.0f, 0.0f, 3.0f);
    camera_->SetOrigin(0, 0, 0);
    ///////////////////////////////////////////
    auto model = render::Model::loadFromFile(
        "resources/lock.obj", render::MeshConfig{.changeable = render::MeshConfig::Dynamic, .triangulate = true});

    model->Scale(0.5, 0.5, 0.5);
    model->texture = data::PngTexture::loadFromFile("resources/cube.png")->getTexture({0, 0});
    models[model->Id()] = std::move(model);

	InitGui();
    BindEvents();
}

void EditorStage::Run() {
	PerformPendingVertexMovement();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        opengl_context_->PickingTexture.Bind();
        current_draw_mode_->DrawPicker(models);
        gizmo.DrawPicking();
        opengl_context_->PickingTexture.Unbind();
    }
    PollEvents();
    current_draw_mode_->Draw(models);
    gizmo.Draw();
    FrameEnd();
}
