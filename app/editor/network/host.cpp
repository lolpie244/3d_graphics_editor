#include "network.h"

Host::Host() {
    socket = std::make_unique<tcp_socket::ConnectionSocket>(settings::PORT);

    socket->listen([this](tcp_socket::CommunicationSocket socket) {
        bool recieve_successful;
        do {
            auto future = socket.on_recieve<bool>(
                [this](std::stringstream& message, const tcp_socket::CommunicationSocket& socket) {
                    ReceiveData(message, socket);
                    return true;
                });
            future.wait();
            recieve_successful = future.get();
        } while (recieve_successful);
    });
}

void Host::SendData(const std::string& data) {
    for (auto& socket : clients_sockets) { socket->send(data); }
}

void Host::ReceiveData(std::stringstream& data, const tcp_socket::CommunicationSocket& socket) {
    static const std::unordered_map<unsigned int, EventHandler> events{
        {},
    };

    unsigned int event;
    data >> event;
    if (events.contains(event))
        events.at(event)(this, socket, data);
    else
        Collaborator::ReceiveData(data);
}
