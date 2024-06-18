#pragma once

#include <alpaca/alpaca.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>
#include <functional>
#include <future>
#include <sstream>
#include <string>
#include <vector>

#include "communication_socket.h"
#include "file_descriptor.h"

namespace tcp_socket {
class CommunicationSocketType;

using BytesType = std::vector<uint8_t>;
using CommunicationSocket = std::shared_ptr<CommunicationSocketType>;
using CommunicationSocketPtr = CommunicationSocketType *;

class CommunicationSocketType {
    static constexpr int BATCH_SIZE = 10000;

   private:
    FileDescriptor socket_fd;
    sockaddr_storage client_address_;
    sockaddr_storage own_address_;
    bool is_server;

   public:
    static CommunicationSocket Create(FileDescriptor socket_fd, sockaddr_storage address, bool is_server) {
        return std::shared_ptr<CommunicationSocketType>(new CommunicationSocketType(socket_fd, address, is_server));
    }

    ~CommunicationSocketType() { stop(); }

    bool operator==(const CommunicationSocketType &other_socket) { return socket_fd == other_socket.socket_fd; }

    template <typename T>
    void send(const T &data) const {
        BytesType bytes;
        auto bytes_written = alpaca::serialize(data, bytes);
        send(bytes);
    }

    void send(const BytesType &data) const {
		int size = data.size();
        BytesType send_bytes;
		send_bytes.insert(send_bytes.end(), reinterpret_cast<uint8_t*>(&size), reinterpret_cast<uint8_t*>(&size) + sizeof(size));
        send_bytes.insert(send_bytes.end(), data.begin(), data.end());

        int send_size = ::send(socket_fd, send_bytes.data(), send_bytes.size(), 0);
        if (send_size == -1) {
            fprintf(stderr, "send: %s\n", strerror(errno));
            return;
        }
    }

    template <typename DataType>
    std::future<std::optional<DataType>> recieve() {
        return std::async(std::launch::async, [&]() {
            int package_size;
            int recieve_size = ::recv(socket_fd, &package_size, sizeof(package_size), 0);

            if (recieve_size <= 0) {
                fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
                return std::optional<DataType>(std::nullopt);
            }

            BytesType bytes(package_size);
            auto current_byte = bytes.data();

            while (package_size) {
                int recieve_size = ::recv(socket_fd, current_byte, std::min(package_size, BATCH_SIZE), 0);

                if (recieve_size <= 0) {
                    fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
                    return std::optional<DataType>(std::nullopt);
                }
                current_byte += recieve_size;
                package_size -= recieve_size;
            }

            return serialize(bytes, (DataType *)nullptr);
        });
    }

    template <typename DataType>
    void start_recieve_loop(std::function<void(const DataType &data)> callback_function) {
        recieve_loop = std::async(std::launch::async, [this, callback_function]() {
            while (true) {
                std::future<std::optional<DataType>> future = this->recieve<DataType>();
                auto value = future.get();
                if (!value.has_value()) {
                    break;
                }
                callback_function(value.value());
            }
        });
    }

    void wait() {
        if (recieve_loop.valid())
            recieve_loop.wait();
    }

    void stop() { ::close(socket_fd); }

    const sockaddr_storage client_address() { return client_address_; }
    const sockaddr_storage own_address() { return own_address_; }

   private:
    CommunicationSocketType(FileDescriptor socket_fd, sockaddr_storage address, bool is_server)
        : socket_fd(socket_fd), client_address_(address), is_server(is_server) {
        socklen_t address_len = sizeof(own_address_);
        getsockname(socket_fd, (struct sockaddr *)&own_address_, &address_len);
    }

    template <typename DataType>
    std::optional<DataType> serialize(const BytesType &bytes, DataType *) {
        std::error_code ec;
        auto data = alpaca::deserialize<DataType>(bytes, ec);
        if (ec) {
            fprintf(stderr, "on_recieve error %s\n", ec.message());
            return std::optional<DataType>(std::nullopt);
        }

        return std::optional<DataType>(data);
    }

    std::optional<BytesType> serialize(const BytesType &bytes, BytesType *) { return std::optional<BytesType>(bytes); }

    std::future<void> recieve_loop;
};

}  // namespace tcp_socket
