#pragma once

#include <netdb.h>

#include <functional>
#include <future>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "communication_socket.h"
#include "file_descriptor.h"

namespace tcp_socket {
using std::string;

class CommunicationSocket {
   private:
    FileDescriptor socket_fd;
    sockaddr_storage address;
    bool is_server;

   public:
    CommunicationSocket(FileDescriptor socket_fd, sockaddr_storage address, bool is_server);
    ~CommunicationSocket();

    bool operator==(const CommunicationSocket& other_socket);

    void close_connection() const;

    void send(const char* message) const;
    void send(std::stringstream& message) const;
    void send(const string& message) const;

    template <typename FuncReturnType>
    std::future<FuncReturnType> on_recieve(
        std::function<FuncReturnType(char* message, const CommunicationSocket& socket)> callback_function);
    template <typename FuncReturnType>
    std::future<FuncReturnType> on_recieve(
        std::function<FuncReturnType(std::stringstream& message, const CommunicationSocket& socket)> callback_function);
    template <typename FuncReturnType>
    std::future<FuncReturnType> on_recieve(
        std::function<FuncReturnType(std::string& message, const CommunicationSocket& socket)> callback_function);
};
}  // namespace tcp_socket
