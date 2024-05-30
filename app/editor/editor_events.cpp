#include <GL/glew.h>
#include <tinyfiledialogs.h>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iterator>

#include "data/model_loader.h"
#include "editor.h"
#include "editor/network/network.h"
#include "math/points_cast.h"
#include "math/ray.h"
#include "math/utils.h"
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

        SendUpdate([this, vertex_info, move]() { connection_->SendVertexMoved(vertex_info, move); });
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

bool EditorStage::OpenScene() {
    std::thread([this]() {
        std::array<const char*, 1> formats{settings::FILE_FORMAT};
        const char* filename =
            tinyfd_openFileDialog("Оберіть файл", "", formats.size(), formats.data(), "Model files", false);

        if (!filename)
            return;

        this->PendingFunctions.push_back([this, filename]() { AddModelFromFile(filename); });
    }).detach();

    return true;
}

bool EditorStage::SaveAsScene() {
    std::thread([this]() {
        std::array<const char*, 1> formats{settings::FILE_FORMAT};
        const char* filename = tinyfd_saveFileDialog("Зберегти файл", "", formats.size(), formats.data(), "Model file");

        if (!filename)
            return;

        std::ofstream file(filename, std::ios::binary);
        auto data = models.begin()->second->toBytes();
        file.write((char*)data.data(), data.size());
    }).detach();

    return true;
}

bool EditorStage::ImportModel() {
    std::thread([this]() {
        std::array<const char*, 2> formats{"*.obj", settings::FILE_FORMAT};
        const char* filename =
            tinyfd_openFileDialog("Оберіть файл", "", formats.size(), formats.data(), "obj files", false);

        if (!filename)
            return;

        this->PendingFunctions.push_back([this, filename]() { AddModelFromFile(filename); });
    }).detach();

    return true;
}
