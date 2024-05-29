#include "utils/alpaca_types.h"

#include "editor/editor.h"
#include "network.h"

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

void Collaborator::ReceiveData(const EventData& event) {
    static const std::unordered_map<unsigned int, EventHandler> events{
        {Events::VertexMove, &Collaborator::VertexMovedHandler},
    };

    if (events.contains(event.event))
        events.at(event.event)(this, event.data);
}
