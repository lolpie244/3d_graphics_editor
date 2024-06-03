#include "network/connection_socket.h"

#include <sys/socket.h>

#include <cstring>

#include "network/communication_socket.h"
#include "utils/settings.h"

namespace tcp_socket {
using std::string;
ConnectionSocket::ConnectionSocket(const char *host, const char *port, const addrinfo &server_address) {
    int status = getaddrinfo(host, port, &server_address, &address);

    socket_fd = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
}

ConnectionSocket::ConnectionSocket(const char *port) {
    addrinfo hints = ConnectionSocket::get_default_addrinfo();
    hints.ai_family = settings::INET_FAMILY;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(NULL, port, &hints, &address);
    socket_fd = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
}

void ConnectionSocket::bind() {
    int status = ::bind(socket_fd, address->ai_addr, address->ai_addrlen);
    if (status == -1)
        throw std::runtime_error(strerror(errno));
}

CommunicationSocket ConnectionSocket::connect() {
    int status = ::connect(socket_fd, address->ai_addr, address->ai_addrlen);
    if (status == -1)
        throw std::runtime_error(strerror(errno));

    return CommunicationSocketType::Create(socket_fd, *(sockaddr_storage *)address->ai_addr, false);
}
void ConnectionSocket::listen(std::function<void(CommunicationSocket communication_socket)> after_accept,
                              int queue_size) {
    if (!is_binded)
        bind();

    ::listen(socket_fd, queue_size);
    sockaddr_storage client_address;
    socklen_t client_address_size = sizeof(client_address);
    while (true) {
        int new_socket_fd = ::accept(socket_fd, NULL, &client_address_size);
        if (new_socket_fd == -1) {
            std::cout << "Accept error: " << strerror(errno) << '\n';
            break;
        }
        threads.emplace_back(
            std::async(std::launch::async, after_accept,
                       std::move(CommunicationSocketType::Create(new_socket_fd, client_address, true))));
    }
}

addrinfo ConnectionSocket::get_default_addrinfo() {
    addrinfo result;
    memset(&result, 0, sizeof(result));
    result.ai_socktype = SOCK_STREAM;
    return result;
}

ConnectionSocket::~ConnectionSocket() {
    // ::close(socket_fd);
    ::freeaddrinfo(address);
}
}  // namespace tcp_socket
