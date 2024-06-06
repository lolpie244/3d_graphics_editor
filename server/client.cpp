#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>

#include "events.h"
#include "network/network.h"

int main() {
    addrinfo hints = tcp_socket::ConnectionSocket::get_default_addrinfo();
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;

    tcp_socket::ConnectionSocket host_socket = tcp_socket::ConnectionSocket(nullptr, PORT, hints);
    host_socket.allow_reuse();
    auto socket = host_socket.connect();

    socket->send(ServerEvent{"123456"});
    auto response = socket->recieve<ClientEvent>(PACKAGE_SIZE).get().value();

    if (response.status == Status::Failed)
        return 1;
    if (response.status == Status::Server) {
        auto listen_socket = std::make_unique<tcp_socket::ConnectionSocket>(socket->own_address());
        listen_socket->allow_reuse();
        auto listener = listen_socket->listen([&](tcp_socket::CommunicationSocket socket) { std::cout << "HEY\n"; });
    } else if (response.status == Status::Client) {
        tcp_socket::ConnectionSocket other_socket = tcp_socket::ConnectionSocket(response.GetAddress());
        while (true) {
            try {
                other_socket.connect();
            } catch (...) {}
        }
    }
}
