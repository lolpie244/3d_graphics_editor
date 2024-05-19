#include <string>

#include "network.h"

Client::Client(EditorStage* stage) : Collaborator(stage) {
    addrinfo hints = tcp_socket::ConnectionSocket::get_default_addrinfo();
    hints.ai_family = settings::INET_FAMILY;
    hints.ai_flags = AI_PASSIVE;

    tcp_socket::ConnectionSocket host_socket(nullptr, settings::PORT, hints);
    this->socket = host_socket.connect();

    SendData(std::to_string(Host_ConnectionAttempt));
    listener = std::async(std::launch::async, [this]() {
        bool recieve_successful;
        do {
            auto future = socket->on_recieve<bool>(
                [this](std::stringstream& message, const tcp_socket::CommunicationSocket& socket) {
                    ReceiveData(message);
                    return true;
                });
            future.wait();
            recieve_successful = future.get();
        } while (recieve_successful);
    });
}

void Client::SendData(const std::string& data) { this->socket->send(data); }
