#include <string>

#include "network.h"
#include "network/communication_socket.h"

Client::Client(EditorStage* stage) : Collaborator(stage) {
    addrinfo hints = tcp_socket::ConnectionSocket::get_default_addrinfo();
    hints.ai_family = settings::INET_FAMILY;
    hints.ai_flags = AI_PASSIVE;

    tcp_socket::ConnectionSocket host_socket(nullptr, settings::PORT, hints);
    this->socket = host_socket.connect();

    SendData(Host_ConnectionAttempt);
    listener = std::async(std::launch::async, [this]() {
        bool recieve_successful;
        do {
            auto future = socket->on_recieve<bool>(
                [this](const tcp_socket::BytesType bytes, const tcp_socket::CommunicationSocket& socket) {
                    ReceiveData(ReceiveBytes(bytes));
                    return true;
                });
            future.wait();
            recieve_successful = future.get();
        } while (recieve_successful);
    });
}

void Client::SendBytes(const tcp_socket::BytesType& data) { this->socket->send(data); }
