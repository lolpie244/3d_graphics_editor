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
#include "utils/alpaca_types.h"
#include "utils/settings.h"

namespace tcp_socket {
using BytesType = std::vector<uint8_t>;

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

    template <typename T>
    void send(const T &data) const {
        BytesType bytes;
        auto bytes_written = alpaca::serialize(data, bytes);
        send(bytes);
    }

    void send(const BytesType &data) const {
        int send_size = ::send(socket_fd, data.data(), data.size(), 0);
        if (send_size == -1) {
            std::cout << "Send error: " << strerror(errno);
            return;
        }
    }

    template <typename FuncReturnType>
    std::future<FuncReturnType> on_recieve(
        std::function<FuncReturnType(const BytesType &data, const CommunicationSocket &socket)> callback_function) {
        BytesType data;
        int recieve_size = ::recv(socket_fd, data.data(), settings::PACKAGE_SIZE, 0);

        if (recieve_size <= 0) {
            fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
            return std::async(std::launch::async, []() { return FuncReturnType(); });
        }
        return std::async(std::launch::async, callback_function, data, *this);
    }
};

}  // namespace tcp_socket
