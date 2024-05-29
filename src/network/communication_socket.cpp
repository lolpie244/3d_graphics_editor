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

CommunicationSocket::~CommunicationSocket() {
    // if (is_server)
    //     ::close(socket_fd);
}
}  // namespace tcp_socket
