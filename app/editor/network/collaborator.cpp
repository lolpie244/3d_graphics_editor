#include "network.h"

std::istream& operator>>(std::istream& is, render::PickingTexture::Info& vertex) {
    is >> vertex.ObjectID >> vertex.VertexId >> vertex.Type;
    return is;
}

std::ostream& operator<<(std::ostream& os, const render::PickingTexture::Info& vertex) {
    os << vertex.ObjectID << ' ' << vertex.VertexId << ' ' << vertex.Type;
    return os;
}

std::istream& operator>>(std::istream& is, glm::vec3& vertex) {
    is >> vertex.x >> vertex.y >> vertex.z;
    return is;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vertex) {
    os << vertex.x << ' ' << vertex.y << ' ' << vertex.z;
    return os;
}

void Collaborator::SendVertexMoved(render::PickingTexture::Info vertex, glm::vec3 moved_to) {
    std::stringstream data;
    data << Events::VertexMove << ' ' << vertex << ' ' << moved_to << '\n';
    SendData(data.str());
}

void Collaborator::VertexMovedHandler(std::stringstream& data) { std::cout << "Receive data " << data.str() << '\n'; }

void Collaborator::ReceiveData(std::stringstream& data) {
    static const std::unordered_map<unsigned int, EventHandler> events{
        {Events::VertexMove, &Collaborator::VertexMovedHandler},
    };

    unsigned int event;
    data >> event;
    events.at(event)(this, data);
}
