#include "network.h"

Client::Client() {
    addrinfo hints = tcp_socket::ConnectionSocket::get_default_addrinfo();
    hints.ai_family = settings::INET_FAMILY;
    hints.ai_flags = AI_PASSIVE;

    tcp_socket::ConnectionSocket host_socket(nullptr, settings::PORT, hints);
    this->socket = host_socket.connect();
}
