#include <future>
#include <string>

#include "editor/editor.h"
#include "network.h"
#include "network/communication_socket.h"
#include "utils/settings.h"

Host::Host(EditorStage* stage, sockaddr_storage address) : Collaborator(stage) {
    socket = std::make_unique<tcp_socket::ConnectionSocket>(address);
    socket->allow_reuse();

    listener = socket->listen([this](tcp_socket::CommunicationSocket socket) { NewClient(socket); });
}

void Host::SendEvent(const EventData& data) {
    for (auto& socket : clients_sockets) { socket->send(data); }
}

void Host::ReceiveData(const EventData& event, tcp_socket::CommunicationSocket& socket) {
    static const std::unordered_map<unsigned int, EventHandler> events{
        // {Event_Host_ConnectionAttempt, &Host::NewConnection},
    };

    if (events.contains(event.event)) {
        events.at(event.event)(this, socket, event.data);
    } else {
        Collaborator::ReceiveData(event);
        for (auto& next_socket : clients_sockets) {
            if (next_socket != socket) {
                next_socket->send(event);
            }
        }
    }
}

void Host::NewClient(tcp_socket::CommunicationSocket socket) {
    clients_sockets.push_back(socket);
    auto it = std::prev(clients_sockets.end(), 1);

    socket->send(EventData(Event_ClientConnected, stage->SceneToBytes()));
    socket->start_recieve_loop<EventData>([this, &socket](const EventData& event) { ReceiveData(event, socket); });
    socket->wait();
    clients_sockets.erase(it);
}
