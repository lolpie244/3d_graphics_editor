#include <future>
#include <iostream>
#include <string>

#include "network/communication_socket.h"
#include "network/network.h"
#include "network/tcp_hole/events.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Need to pass server port";
    }

    addrinfo hints = tcp_socket::ConnectionSocket::get_default_addrinfo();
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;

    auto socket = std::make_unique<tcp_socket::ConnectionSocket>(nullptr, argv[1], hints);

    std::unordered_map<std::string, tcp_socket::CommunicationSocket> groups;

    tcp_socket::CommunicationSocket server;
    auto listener = socket->listen([&](tcp_socket::CommunicationSocket socket) {
        auto data = socket->recieve<tcp_hole::ServerEvent>().get();
        if (!data.has_value()) {
            socket->send(tcp_hole::ClientEvent(tcp_hole::Status::Failed));
            return;
        }

        std::string group = data.value().data;
        std::cout << group << '\n';

        if (!groups.count(group)) {
            groups.insert({group, socket});
            socket->send(tcp_hole::ClientEvent(tcp_hole::Status::Server));
            return;
        }
        socket->send(tcp_hole::ClientEvent(tcp_hole::Status::Client, groups.at(group)->client_address()));
    });
}
