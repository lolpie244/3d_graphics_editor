#include <GL/glew.h>
#include <tinyfiledialogs.h>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iterator>
#include <system_error>

#include "alpaca/alpaca.h"
#include "data/model_loader.h"
#include "editor.h"
#include "editor/network/network.h"
#include "math/points_cast.h"
#include "math/ray.h"
#include "math/utils.h"
#include "network/communication_socket.h"
#include "render/light.h"
#include "render/model.h"
#include "utils/settings.h"

bool EditorStage::CameraMove(sf::Event event, glm::vec2 moved) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        auto move = math::to_ndc(stage::StageManager::Instance().windowSize() / 2.0f + moved) * Scale();
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
    Camera()->Move(0, 0, -event.mouseWheelScroll.delta * Scale());
    Camera()->SetOrigin(Camera()->GetOrigin().x, Camera()->GetOrigin().y, old_origin.z);

    scale_ = scale_ - 0.5 * event.mouseWheelScroll.delta;
    return true;
}

void EditorStage::ClearSelection() {
    current_gizmo_->Reset();
    for (auto& vertex : selected_vertexes_) {
        models[vertex.ObjectID]->SetVertexColor(vertex.VertexId, vertex.Type, settings::DEFAULT_POINT_COLOR);
    }
    selected_vertexes_.clear();
}

void EditorStage::Select(render::PickingTexture::Info info) {
    auto model = models[info.ObjectID].get();

    if (info.Type == render::Model::Surface) {
        current_gizmo_->SetModel(model);
        return;
    }

    model->SetVertexColor(info.VertexId, info.Type, settings::SELECTED_POINT_COLOR);
    selected_vertexes_.insert(info);
}

bool EditorStage::ContextPress(sf::Event event) {
    pending_move_ = false;
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        ClearSelection();

    selection_rect_->Enable();
    return true;
}

bool EditorStage::ContextDrag(sf::Event event, glm::vec2 move) {
    glm::vec2 start = Context()->StartPosition();
    glm::vec2 end = glm::vec2{event.mouseMove.x, event.mouseMove.y};
    auto size = end - start;

    selection_rect_->SetRect(start.x, start.y, size.x, size.y);
    return true;
}

bool EditorStage::ContextRelease(sf::Event event) {
    selection_rect_->Disable();
    auto rect = selection_rect_->Rect();
    auto position = glm::vec2(rect.left, rect.top);
    auto size = glm::vec2(rect.width, rect.height);

    for (auto& pixel : Context()->PickingTexture.ReadArea(position.x, position.y, size.x, size.y)) {
        if (pixel.ObjectID == 0 || pixel.Type == render::Model::Surface)
            continue;
        Select(pixel);
    }
    return true;
}

bool EditorStage::ModelPress(sf::Event event, render::Model* model) {
    if (pending_move_) {
        ClearSelection();
        pending_move_ = false;
        return true;
    }

    last_vertex_position = {-1, -1, -1};

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !selected_vertexes_.contains(model->PressInfo()))
        ClearSelection();

    Select(model->PressInfo());
    return true;
}

bool EditorStage::MoveSelectedPoints(sf::Event event, render::PickingTexture::Info press_info) {
    if (press_info.Type != render::Model::Point && press_info.Type != render::Model::Pending)
        return false;

    auto model = models[press_info.ObjectID].get();

    math::Ray ray = math::Ray::FromPoint({event.mouseMove.x, event.mouseMove.y});
    render::ModelVertex vertex;

    auto current_vertex_position = last_vertex_position;

    if (current_vertex_position == glm::vec3{-1, -1, -1}) {
        current_vertex_position = glm::vec4(model->Vertex(press_info.VertexId, press_info.Type).position, 1.0f);
    }

    current_vertex_position = model->GetTransformation() * glm::vec4(current_vertex_position, 1.0f);

    auto intersect_point = ray.SphereIntersection(current_vertex_position);
    intersect_point = glm::inverse(model->GetTransformation()) * glm::vec4(intersect_point, 1.0f);

    if (this->last_vertex_position == glm::vec3(-1, -1, -1)) {
        this->last_vertex_position = intersect_point;
        return true;
    }
    auto move = intersect_point - last_vertex_position;

    for (auto& vertex_info : selected_vertexes_) {
        auto* model = models[vertex_info.ObjectID].get();
        model->SetVertexPosition(vertex_info.VertexId, vertex_info.Type,
                                 model->Vertex(vertex_info.VertexId, vertex_info.Type).position + move);

        SendRequest([vertex_info, move](Collaborator* connection) { connection->SendVertexMoved(vertex_info, move); });
    }

    this->last_vertex_position = intersect_point;
    if (selected_vertexes_.size() <= settings::DYNAMIC_TRIANGULATE_LIMIT)
        model->Triangulate(selected_vertexes_);

    return true;
}

bool EditorStage::ModelRelease(sf::Event event, render::Model* model) {
    model->Triangulate(selected_vertexes_);
    return true;
}

bool EditorStage::DeleteModel(sf::Event event) {
    if (!current_gizmo_->GetModel())
        return false;
    auto id = current_gizmo_->GetModel()->Id();
    models.erase(id);
    lights.erase(id);
    current_gizmo_->Reset();

    return true;
}

bool EditorStage::LightPress(sf::Event event, render::Light* light) {
    ClearSelection();
    current_gizmo_->SetModel(light);
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
    pending_move_ = true;
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
    ClearSelection();
    return true;
}

void EditorStage::PerformPendingFunctions() {
    for (auto& function : PendingFunctions) { function(); }
    PendingFunctions.clear();
}

void EditorStage::AddLight(std::unique_ptr<render::Light> light) {
    if (lights.size() >= settings::MAXIMUM_LIGHT_COUNT)
        return;

    light->BindPress(observer_, [this, light = light.get()](sf::Event event) { return LightPress(event, light); },
                     {sf::Mouse::Left});
    lights.insert({light->Id(), std::move(light)});
}

bool EditorStage::AddLight(sf::Event event) {
    RunAsync([this]() {
        unsigned char lRgbColor[3];
        if (!tinyfd_colorChooser("Оберіть колір", "#FFFFFF", lRgbColor, lRgbColor))
            return;

        PendingFunctions.push_back([this, lRgbColor]() {
            glm::vec4 color = {lRgbColor[0] / 255.0f, lRgbColor[1] / 255.0f, lRgbColor[2] / 255.0f, 1};
            auto data = DEFAULT_LIGHT_DATA;
            data.color = color;
            auto light = std::make_unique<render::Light>(data);

            this->AddLight(std::move(light));
        });
    });
    return true;
}

void EditorStage::LoadScene(const tcp_socket::BytesType& data) {
    std::error_code ec;
    auto scenes = alpaca::deserialize<SceneData>(data, ec);

    for (auto& model : scenes.models) { AddModel(render::Model::fromBytes(model, DEFAULT_MODEL_CONFIG)); }
    for (auto& light : scenes.ligths) { AddLight(render::Light::fromBytes(light)); }
}

bool EditorStage::NewScene() {
    Clear();
    current_filename_ = nullptr;
    return true;
}

bool EditorStage::OpenScene() {
    RunAsync([this]() {
        std::array<const char*, 1> formats{settings::FILE_FORMAT};
        this->Clear();

        const char* filename =
            tinyfd_openFileDialog("Оберіть файл", "", formats.size(), formats.data(), "Model files", false);

        if (!filename)
            return;

        this->PendingFunctions.push_back([this, filename]() {
            std::ifstream input(filename, std::ios::binary);
            std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
            LoadScene(buffer);
        });
    });

    return true;
}

bool EditorStage::SaveScene() {
    if (current_filename_ == nullptr)
        SaveAsScene();

    RunAsync([this]() {
        std::ofstream file(current_filename_, std::ios::binary);
        SceneData scenes;
        for (auto& [_, model] : models) { scenes.models.push_back(model->toBytes()); }
        for (auto& [_, light] : lights) { scenes.ligths.push_back(light->toBytes()); }

        tcp_socket::BytesType bytes;
        alpaca::serialize(scenes, bytes);
        file.write((char*)bytes.data(), bytes.size());
    });

    return true;
}

bool EditorStage::SaveAsScene() {
    RunAsync([this]() {
        std::array<const char*, 1> formats{settings::FILE_FORMAT};
        const char* filename = tinyfd_saveFileDialog("Зберегти файл", "", formats.size(), formats.data(), "Model file");

        if (!filename)
            return;

        SetFilename(filename);
        SaveScene();
    });

    return true;
}

bool EditorStage::ImportModel() {
    RunAsync([this]() {
        std::array<const char*, 1> formats{"*.obj"};
        const char* filename =
            tinyfd_openFileDialog("Оберіть файл", "", formats.size(), formats.data(), "obj files", false);

        if (!filename)
            return;

        this->PendingFunctions.push_back([this, filename]() {
            auto model = render::Model::loadFromFile(filename, DEFAULT_MODEL_CONFIG);
            AddModel(std::move(model));
        });
    });

    return true;
}
