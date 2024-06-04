#include <alpaca/alpaca.h>

#include <memory>

#include "editor/editor.h"
#include "math/transform.h"
#include "network.h"
#include "network/communication_socket.h"
#include "render/light.h"
#include "render/model.h"

Collaborator::Collaborator(EditorStage* stage) : stage(stage) {}

struct VertexMovedData {
    render::PickingTexture::Info vertex;
    glm::vec3 new_position;
};

struct NewModelData {
    uint8_t id;
    tcp_socket::BytesType bytes;
};

struct ModelTransformData {
    uint8_t id;

    math::TransformData localTransform;
    math::TransformData globalTransform;
};

struct LightTransformData {
    uint8_t id;

    math::TransformData localTransform;
};

void Collaborator::VertexMoved(render::PickingTexture::Info vertex, glm::vec3 new_position) {
    SendEvent(EventData(Event_VertexMove, VertexMovedData{.vertex = vertex, .new_position = new_position}));
}

void Collaborator::VertexMovedHandler(const tcp_socket::BytesType& raw_data) {
    std::error_code ec;
    auto data = alpaca::deserialize<VertexMovedData>(raw_data, ec);

    stage->PendingFunctions.push_back([vertex = data.vertex, new_position = data.new_position, this]() {
        auto* model = stage->models.at(vertex.ObjectID).get();
        model->SetVertexPosition(vertex.VertexId, vertex.Type, new_position);
    });
}

void Collaborator::NewModel(render::Model* model) {
    SendEvent(EventData(Event_ModelAdd, NewModelData{.id = model->Id(), .bytes = model->toBytes()}));
}

void Collaborator::NewModelHandler(const tcp_socket::BytesType& raw_data) {
    std::error_code ec;
    auto data = alpaca::deserialize<NewModelData>(raw_data, ec);

    stage->PendingFunctions.push_back([this, data, raw_data]() {
        auto model = render::Model::fromBytes(data.bytes, EditorStage::DEFAULT_MODEL_CONFIG);
        model->ForceSetId(data.id);
        stage->AddModel(std::move(model), false);
    });
}

void Collaborator::ModelTransform(render::Model* model) {
    SendEvent(EventData(Event_ModelTransform,
                        ModelTransformData{.id = model->Id(),
                                           .localTransform = model->GetTransformData(),
                                           .globalTransform = model->GlobalTransform.GetTransformData()}));
}
void Collaborator::ModelTransformHandler(const tcp_socket::BytesType& raw_data) {
    std::error_code ec;
    auto data = alpaca::deserialize<ModelTransformData>(raw_data, ec);
    stage->models.at(data.id)->SetTransformData(data.localTransform);
    stage->models.at(data.id)->GlobalTransform.SetTransformData(data.globalTransform);
}

void Collaborator::NewLight(render::Light* light) {
    SendEvent(EventData(Event_LightAdd, NewModelData{.id = light->Id(), .bytes = light->toBytes()}));
}

void Collaborator::NewLightHandler(const tcp_socket::BytesType& raw_data) {
    std::error_code ec;
    auto data = alpaca::deserialize<NewModelData>(raw_data, ec);

    stage->PendingFunctions.push_back([this, data, raw_data]() {
        auto light = render::Light::fromBytes(data.bytes);
        light->ForceSetId(data.id);
        stage->AddLight(std::move(light), false);
    });
}

void Collaborator::LightTransform(render::Light* model) {
    SendEvent(EventData(Event_LightTransform,
                        LightTransformData{.id = model->Id(), .localTransform = model->GetTransformData()}));
}
void Collaborator::LightTransformHandler(const tcp_socket::BytesType& raw_data) {
    std::error_code ec;
    auto data = alpaca::deserialize<LightTransformData>(raw_data, ec);
    stage->lights.at(data.id)->SetTransformData(data.localTransform);
}

void Collaborator::ReceiveData(const EventData& event) {
    static const std::unordered_map<unsigned int, EventHandler> events{
        {Event_VertexMove, &Collaborator::VertexMovedHandler},        {Event_ModelAdd, &Collaborator::NewModelHandler},
        {Event_ModelTransform, &Collaborator::ModelTransformHandler}, {Event_LightAdd, &Collaborator::NewLightHandler},
        {Event_LightTransform, &Collaborator::LightTransformHandler},
    };

    if (events.contains(event.event))
        events.at(event.event)(this, event.data);
    else {
        std::cout << "INCORRECT EVENT\n";
    }
}
