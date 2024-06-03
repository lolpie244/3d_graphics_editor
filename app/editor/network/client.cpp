#include <string>

#include "network.h"
#include "network/communication_socket.h"
#include "network/connection_socket.h"

Client::Client(EditorStage* stage) : Collaborator(stage) {
    addrinfo hints = tcp_socket::ConnectionSocket::get_default_addrinfo();
    hints.ai_family = settings::INET_FAMILY;
    hints.ai_flags = AI_PASSIVE;

    tcp_socket::ConnectionSocket host_socket = tcp_socket::ConnectionSocket(nullptr, settings::PORT, hints);
    this->socket = host_socket.connect();

    SendEvent(Event_Host_ConnectionAttempt);
    listener = std::async(std::launch::async, [this]() {
        bool recieve_successful;
        do {
            auto future = socket->on_recieve<bool, EventData>(
                [this](const EventData& bytes) {
                    ReceiveData(bytes);
                    return true;
                });
            future.wait();
            recieve_successful = future.get();
        } while (recieve_successful);
    });
}

void Client::SendEvent(const EventData& event) { socket->send(event); }
