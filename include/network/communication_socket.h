#pragma once

#include <netdb.h>

#include <cstring>
#include <functional>
#include <future>
#include <sstream>
#include <string>
#include <vector>

#include "communication_socket.h"
#include "file_descriptor.h"
#include "utils/alpaca_types.h"
#include "utils/settings.h"

namespace tcp_socket {
class CommunicationSocketType;

using BytesType = std::vector<uint8_t>;
using CommunicationSocket = std::shared_ptr<CommunicationSocketType>;
using CommunicationSocketPtr = CommunicationSocketType*;

class CommunicationSocketType {
   private:
    FileDescriptor socket_fd;
    sockaddr_storage address;
    bool is_server;

   public:
    static CommunicationSocket Create(FileDescriptor socket_fd, sockaddr_storage address,
                                                           bool is_server) {
        return std::shared_ptr<CommunicationSocketType>(new CommunicationSocketType(socket_fd, address, is_server));
    }

    ~CommunicationSocketType() = default;

    bool operator==(const CommunicationSocketType &other_socket) { return socket_fd == other_socket.socket_fd; }

    template <typename T>
    void send(const T &data) const {
        BytesType bytes;
        auto bytes_written = alpaca::serialize(data, bytes);
        send(bytes);
    }

    void send(const BytesType &data) const {
        int send_size = ::send(socket_fd, data.data(), data.size(), 0);
        if (send_size == -1) {
			fprintf(stderr, "on_recieve error %s\n", strerror(errno));
            return;
        }
    }

    template <typename FuncReturnType, typename DataType>
    std::future<FuncReturnType> on_recieve(
        std::function<FuncReturnType(const DataType &data)> callback_function) {
        BytesType bytes(settings::PACKAGE_SIZE);
        int recieve_size = ::recv(socket_fd, bytes.data(), settings::PACKAGE_SIZE, 0);

        if (recieve_size <= 0) {
            fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
            return std::async(std::launch::async, []() { return FuncReturnType(); });
        }
        std::error_code ec;
        auto data = alpaca::deserialize<DataType>(bytes, ec);
        if (ec) {
            fprintf(stderr, "on_recieve error %s\n", ec.message());
            return std::async(std::launch::async, []() { return FuncReturnType(); });
        }

        return std::async(std::launch::async, callback_function, data);
    }

   private:
    CommunicationSocketType(FileDescriptor socket_fd, sockaddr_storage address, bool is_server)
        : socket_fd(socket_fd), address(address), is_server(is_server) {}
};

}  // namespace tcp_socket
