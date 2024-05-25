#include "network/communication_socket.h"

#include <netdb.h>
#include <string.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstring>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "utils/settings.h"

namespace tcp_socket {
CommunicationSocket::CommunicationSocket(FileDescriptor socket_fd, sockaddr_storage address, bool is_server)
    : socket_fd(socket_fd), address(address), is_server(is_server) {}

bool CommunicationSocket::operator==(const CommunicationSocket &other_socket) {
    return socket_fd == other_socket.socket_fd;
}

void CommunicationSocket::close_connection() const { ::close(socket_fd); }

void CommunicationSocket::send(const char *message) const {
    int send_size;
    send_size = ::send(socket_fd, message, settings::PACKAGE_SIZE, 0);
    if (send_size == -1) {
        std::cout << "Send error: " << strerror(errno);
        return;
    }
}

void CommunicationSocket::send(std::stringstream &message) const {
    std::string next_message;
    std::getline(message, next_message, '\0');
    send(next_message.c_str());
}

void CommunicationSocket::send(const std::string &message) const { send(message.c_str()); }

CommunicationSocket::~CommunicationSocket() {
    // if (is_server)
    //     ::close(socket_fd);
}
}  // namespace tcp_socket
