#include "editor.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>
#include <functional>
#include <future>
#include <memory>

#include "data/texture.h"
#include "editor/network/network.h"
#include "gui/select_rect.h"
#include "render/light.h"
#include "render/mesh.h"
#include "render/model.h"
#include "utils/settings.h"

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

    events.push_back(observer_.KeyBind({sf::Keyboard::Delete}, [this](sf::Event event) { return DeleteModel(event); }));

    opengl_context_->BindPress(observer_, [&](sf::Event event) { return ContextPress(event); }, {sf::Mouse::Left});
    opengl_context_->BindRelease(observer_, [&](sf::Event event) { return ContextRelease(event); }, {sf::Mouse::Left});
    opengl_context_->BindDrag(observer_, [&](sf::Event event, glm::vec2 moved) { return ContextDrag(event, moved); },
                              {sf::Mouse::Left});
    opengl_context_->BindDrag(observer_, [&](sf::Event event, glm::vec2 moved) { return CameraMove(event, moved); },
                              {sf::Mouse::Right, sf::Mouse::Middle});
    opengl_context_->BindMouseMove(observer_, [&](sf::Event event) {
        if (!pending_move_ || selected_vertexes_.empty())
            return false;
        return MoveSelectedPoints(event, *selected_vertexes_.begin());
    });
    opengl_context_->BindScroll(observer_, [this](sf::Event event) { return CameraZoom(event); });
}

void EditorStage::AddModel(std::unique_ptr<render::Model> model, bool send_request) {
    model->texture = data::PngTexture::loadFromFile("resources/default/default_texture.png")->getTexture({0, 0});
    model->BindPress(observer_, [this, model = model.get()](sf::Event event) { return ModelPress(event, model); },
                     {sf::Mouse::Left});
    model->BindRelease(observer_, [this, model = model.get()](sf::Event event) { return ModelRelease(event, model); },
                       {sf::Mouse::Left});
    model->BindDrag(observer_,
                    [this, model = model.get()](sf::Event event, glm::vec3 move) {
                        return MoveSelectedPoints(event, model->PressInfo());
                    },
                    {sf::Mouse::Left});

    if (send_request)
        SendRequest([model = model.get()](Collaborator* connection) { connection->NewModel(model); });

    models.insert({model->Id(), std::move(model)});
}

void EditorStage::AddLight(std::unique_ptr<render::Light> light, bool send_request) {
    if (lights.size() >= settings::MAXIMUM_LIGHT_COUNT)
        return;

    light->BindPress(observer_, [this, light = light.get()](sf::Event event) { return LightPress(event, light); },
                     {sf::Mouse::Left});
    if (send_request)
        SendRequest([light = light.get()](Collaborator* connection) { connection->NewLight(light); });

    lights.insert({light->Id(), std::move(light)});
}

void EditorStage::SendRequest(std::function<void(Collaborator*)> func) {
    if (!connection_)
        return;
    RunAsync([func, this]() { func(connection_.get()); });
}

void EditorStage::RunAsync(std::function<void(void)> func) {
    requests_.push_back(std::async(std::launch::async, [this, func]() { func(); }));
}

void EditorStage::SetFilename(const char* filename) { current_filename_ = filename; }

void EditorStage::Clear() {
    ClearSelection();
    models.clear();
    lights.clear();
}

EditorStage::EditorStage() {
    gizmo_shader_.loadFromFile("shaders/color.vert", "shaders/color.frag");
    gizmo_picking_.loadFromFile("shaders/color.vert", "shaders/picking.frag");

    camera_->Move(0, 0, 4.0f);
    camera_->SetOrigin(0, 0, 0);

    InitGui();
    BindEvents();
}

void EditorStage::Run() {
    window_->pushGLStates();
    window_->draw(elements_);
    window_->popGLStates();

    PerformPendingFunctions();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        opengl_context_->PickingTexture.Bind();
        current_draw_mode_->DrawPicker(models, lights);
        current_gizmo_->Draw(gizmo_picking_);
        opengl_context_->PickingTexture.Unbind();
    }
    PollEvents();
    current_draw_mode_->Draw(models, lights);
    current_gizmo_->Draw(gizmo_shader_);
    FrameEnd();
}
