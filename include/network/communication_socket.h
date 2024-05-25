#pragma once

#include <netdb.h>

#include <cstring>
#include <functional>
#include <future>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "communication_socket.h"
#include "file_descriptor.h"
#include "utils/settings.h"

namespace tcp_socket {

class CommunicationSocket {
   private:
    FileDescriptor socket_fd;
    sockaddr_storage address;
    bool is_server;

   public:
    CommunicationSocket(FileDescriptor socket_fd, sockaddr_storage address, bool is_server);
    ~CommunicationSocket();

    bool operator==(const CommunicationSocket &other_socket);

    void close_connection() const;

    void send(const char *message) const;
    void send(std::stringstream &message) const;
    void send(const std::string &message) const;

    template <typename FuncReturnType>
    std::future<FuncReturnType> on_recieve(
        std::function<FuncReturnType(char *message, const CommunicationSocket &socket)> callback_function) {
        char buffer[settings::PACKAGE_SIZE];
        int recieve_size = ::recv(socket_fd, buffer, settings::PACKAGE_SIZE, 0);

        if (recieve_size <= 0) {
			fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
            return std::async(std::launch::async, []() { return FuncReturnType(); });
        }
        return std::async(std::launch::async, callback_function, buffer, *this);
    }

    template <typename FuncReturnType>
    std::future<FuncReturnType> on_recieve(
        std::function<FuncReturnType(std::stringstream &message, const CommunicationSocket &address)>
            callback_function) {
        return on_recieve<FuncReturnType>([callback_function](char *buffer, const CommunicationSocket &socket) {
            std::stringstream message(buffer);
            return callback_function(message, socket);
        });
    }

    template <typename FuncReturnType>
    std::future<FuncReturnType> on_recieve(
        std::function<FuncReturnType(std::string &message, const CommunicationSocket &address)> callback_function) {
        return on_recieve<FuncReturnType>([=](char *buffer, const CommunicationSocket &socket) {
            std::string message(buffer);
            return callback_function(message, socket);
        });
    }
};
}  // namespace tcp_socket
