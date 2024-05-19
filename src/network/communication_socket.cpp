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
using std::string, std::stringstream;

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

void CommunicationSocket::send(stringstream &message) const {
    string next_message;
    std::getline(message, next_message, '\0');
    send(next_message.c_str());
}

void CommunicationSocket::send(const string &message) const { send(message.c_str()); }

template <typename FuncReturnType>
std::future<FuncReturnType> CommunicationSocket::on_recieve(
    std::function<FuncReturnType(char *message, const CommunicationSocket &socket)> callback_function) {
    char buffer[settings::PACKAGE_SIZE];
    int recieve_size = ::recv(socket_fd, buffer, settings::PACKAGE_SIZE, 0);

    if (recieve_size <= 0) {
        return std::async([]() { return FuncReturnType(); });
    }

    return std::async(std::launch::async, callback_function, buffer, *this);
}

template <typename FuncReturnType>
std::future<FuncReturnType> CommunicationSocket::on_recieve(
    std::function<FuncReturnType(stringstream &message, const CommunicationSocket &address)> callback_function) {
    return on_recieve([callback_function](char *buffer, const CommunicationSocket &socket) {
        stringstream message(buffer);
        callback_function(message, socket);
    });
}

template <typename FuncReturnType>
std::future<FuncReturnType> CommunicationSocket::on_recieve(
    std::function<FuncReturnType(string &message, const CommunicationSocket &address)> callback_function) {
    return on_recieve([=](char *buffer, const CommunicationSocket &socket) {
        string message(buffer);
        callback_function(message, socket);
    });
}

CommunicationSocket::~CommunicationSocket() {
    if (is_server)
        ::close(socket_fd);
}
}  // namespace tcp_socket
