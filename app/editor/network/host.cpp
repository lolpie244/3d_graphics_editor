#include <string>

#include "network.h"
#include "network/communication_socket.h"

Host::Host(EditorStage* stage) : Collaborator(stage) {
    socket = std::make_unique<tcp_socket::ConnectionSocket>(settings::PORT);

    listener = std::async(std::launch::async, [this]() {
        socket->listen([this](tcp_socket::CommunicationSocket socket) {
            bool recieve_successful;
            do {
                auto future = socket.on_recieve<bool>(
                    [this](const tcp_socket::BytesType& bytes, const tcp_socket::CommunicationSocket& socket) {
                        ReceiveData(ReceiveBytes(bytes), socket);
                        return true;
                    });
                future.wait();
                recieve_successful = future.get();
            } while (recieve_successful);
        });
    });
}

void Host::SendBytes(const tcp_socket::BytesType& data) {
    for (auto& socket : clients_sockets) { socket.send(data); }
}

void Host::ReceiveData(const EventData& event, const tcp_socket::CommunicationSocket& socket) {
    static const std::unordered_map<unsigned int, EventHandler> events{
        {Host_ConnectionAttempt, &Host::NewConnection},
    };

    if (events.contains(event.event))
        events.at(event.event)(this, socket, event.data);
    else {
        Collaborator::ReceiveData(event);
    }
}

void Host::NewConnection(const tcp_socket::CommunicationSocket& socket, const tcp_socket::BytesType& data) {
    clients_sockets.push_back(socket);
    SendData(Client_Connected);
}
