#include <string>

#include "network.h"
#include "network/communication_socket.h"
#include "utils/settings.h"

Host::Host(EditorStage* stage) : Collaborator(stage) {
    addrinfo hints = tcp_socket::ConnectionSocket::get_default_addrinfo();
    hints.ai_family = settings::INET_FAMILY;
    hints.ai_flags = AI_PASSIVE;

    socket = std::make_unique<tcp_socket::ConnectionSocket>(nullptr, settings::PORT, hints);

    listener = std::async(std::launch::async, [this]() {
        socket->listen([this](tcp_socket::CommunicationSocket socket) {
            bool recieve_successful;
            do {
                auto future = socket->on_recieve<bool, EventData>([this, &socket](const EventData& bytes) {
                    ReceiveData(bytes, socket);
                    return true;
                }, settings::PACKAGE_SIZE);
                future.wait();
                recieve_successful = future.get();
            } while (recieve_successful);
        });
    });
}

void Host::SendEvent(const EventData& data) {
    for (auto& socket : clients_sockets) { socket->send(data); }
}

void Host::ReceiveData(const EventData& event, tcp_socket::CommunicationSocket& socket) {
    static const std::unordered_map<unsigned int, EventHandler> events{
        {Event_Host_ConnectionAttempt, &Host::NewConnection},
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

void Host::NewConnection(tcp_socket::CommunicationSocket& socket, const tcp_socket::BytesType& data) {
    clients_sockets.push_back(socket);
    socket->send(EventData(Event_Client_Connected));
}
