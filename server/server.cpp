#include <future>
#include <iostream>
#include <string>

#include "events.h"
#include "network/communication_socket.h"
#include "network/network.h"

int main() {
    addrinfo hints = tcp_socket::ConnectionSocket::get_default_addrinfo();
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;

    auto socket = std::make_unique<tcp_socket::ConnectionSocket>(nullptr, PORT, hints);

    std::unordered_map<std::string, tcp_socket::CommunicationSocket> groups;

    tcp_socket::CommunicationSocket server;
    auto listener = socket->listen([&](tcp_socket::CommunicationSocket socket) {
        auto data = socket->recieve<ServerEvent>(CODE_LEN).get();
        if (!data.has_value()) {
            socket->send(ClientEvent(Status::Failed));
            return;
        }

        std::string group = data.value().data;
		std::cout << group << '\n';

        if (!groups.count(group)) {
            groups.insert({group, socket});
            socket->send(ClientEvent(Status::Server));
            return;
        }
        socket->send(ClientEvent(Status::Client, groups.at(group)->client_address()));
    });
}
