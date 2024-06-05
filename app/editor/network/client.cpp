#include <string>

#include "network.h"
#include "network/communication_socket.h"
#include "network/connection_socket.h"
#include "utils/settings.h"

Client::Client(EditorStage* stage) : Collaborator(stage) {
    addrinfo hints = tcp_socket::ConnectionSocket::get_default_addrinfo();
    hints.ai_family = settings::INET_FAMILY;
    hints.ai_flags = AI_PASSIVE;

    tcp_socket::ConnectionSocket host_socket = tcp_socket::ConnectionSocket(nullptr, settings::PORT, hints);
    this->socket = host_socket.connect();

    socket->start_recieve_loop<EventData>([this](const EventData& event) { ReceiveData(event); },
                                          settings::PACKAGE_SIZE);
}

void Client::SendEvent(const EventData& event) { socket->send(event); }
