#include <memory>

#include "editor/editor.h"
#include "network.h"
#include "network/communication_socket.h"
#include "render/model.h"
#include "utils/alpaca_types.h"

Collaborator::Collaborator(EditorStage* stage) : stage(stage) {}

struct VertexMovedData {
    render::PickingTexture::Info vertex;
    glm::vec3 moved_to;
};

void Collaborator::SendVertexMoved(render::PickingTexture::Info vertex, glm::vec3 moved_to) {
    SendData(Events::VertexMove, VertexMovedData{.vertex = vertex, .moved_to = moved_to});
}

void Collaborator::VertexMovedHandler(const tcp_socket::BytesType& raw_data) {
    std::error_code ec;
    auto data = alpaca::deserialize<VertexMovedData>(raw_data, ec);

    stage->PendingFunctions.push_back([vertex = data.vertex, moved_to = data.moved_to, this]() {
        auto* model = stage->models.at(vertex.ObjectID).get();
        model->SetVertexPosition(
            vertex.VertexId, vertex.Type,
            model->Vertex(vertex.VertexId, vertex.Type).position + glm::vec3{moved_to[0], moved_to[1], moved_to[2]});
    });
}

struct NewModelData {
    int id;
    tcp_socket::BytesType bytes;
};

void Collaborator::SendNewModel(render::Model* model) {
    SendData(ModelAdd, NewModelData{.id = model->Id(), .bytes = model->toBytes()});
}

void Collaborator::NewModelHandler(const tcp_socket::BytesType& raw_data) {
    std::error_code ec;
    auto data = alpaca::deserialize<NewModelData>(raw_data, ec);

    stage->PendingFunctions.push_back([this, data]() {
        auto model = render::Model::fromBytes(data.bytes, EditorStage::DEFAULT_MODEL_CONFIG);
        model->ForceSetId(data.id);
        stage->AddModel(std::move(model));
    });
}

void Collaborator::ReceiveData(const EventData& event) {
    static const std::unordered_map<unsigned int, EventHandler> events{
        {Events::VertexMove, &Collaborator::VertexMovedHandler},
        {Events::ModelAdd, &Collaborator::NewModelHandler},
    };

    if (events.contains(event.event))
        events.at(event.event)(this, event.data);
}
