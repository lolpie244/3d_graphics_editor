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
}

void EditorStage::AddModel(std::unique_ptr<render::Model> model) {
    model->BindPress(observer_, [this, model = model.get()](sf::Event event) { return ModelPress(event, model); },
                     {sf::Mouse::Left});
    model->BindRelease(observer_, [this, model = model.get()](sf::Event event) { return ModelRelease(event, model); },
                       {sf::Mouse::Left});
    model->BindDrag(
        observer_,
        [this, model = model.get()](sf::Event event, glm::vec3 move) { return ModelDrag(event, move, model); },
        {sf::Mouse::Left});
    models.insert({model->Id(), std::move(model)});
}

EditorStage::EditorStage() {
    gizmo_shader_.loadFromFile("shaders/gizmo.vert", "shaders/gizmo.frag");
    gizmo_picking_.loadFromFile("shaders/gizmo.vert", "shaders/picking.frag");

    camera_->Move(0.0f, 0.0f, 3.0f);
    camera_->SetOrigin(0, 0, 0);

    InitGui();
    BindEvents();
}

void EditorStage::Run() {
    PerformPendingVertexMovement();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        opengl_context_->PickingTexture.Bind();
        current_draw_mode_->DrawPicker(models);
        current_gizmo_->Draw(gizmo_picking_);
        opengl_context_->PickingTexture.Unbind();
    }
    PollEvents();
    current_draw_mode_->Draw(models);
    current_gizmo_->Draw(gizmo_shader_);
    FrameEnd();
}
