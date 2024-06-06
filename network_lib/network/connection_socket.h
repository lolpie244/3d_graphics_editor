#pragma once

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <functional>
#include <future>
#include <memory>
#include <string>
#include <vector>

#include "communication_socket.h"

namespace tcp_socket {
using std::string;

class ConnectionSocket {
   private:
    FileDescriptor socket_fd;
    addrinfo *address;
    bool is_binded = false;

    std::vector<std::future<void>> threads;

   public:
    ConnectionSocket(const char *host, const char *port, const addrinfo &server_address) {
        int status = getaddrinfo(host, port, &server_address, &address);
        socket_fd = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    }

    ConnectionSocket(sockaddr_storage data) {
        sockaddr *sa = (sockaddr *)&data;
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        int result = getnameinfo(sa, sizeof(sockaddr_storage), host, NI_MAXHOST, service, NI_MAXSERV,
                                 NI_NUMERICHOST | NI_NUMERICSERV);

        std::cout << host << ":" << service << '\n';

        addrinfo hints = get_default_addrinfo();
        hints.ai_family = AF_UNSPEC;
        hints.ai_flags = AI_PASSIVE;

        int status = getaddrinfo(host, service, &hints, &address);
        socket_fd = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    }
    void bind() {
        int status = ::bind(socket_fd, address->ai_addr, address->ai_addrlen);
        if (status == -1)
            throw std::runtime_error(strerror(errno));
    }
    CommunicationSocket connect() {
        int status = ::connect(socket_fd, address->ai_addr, address->ai_addrlen);
        if (status == -1)
            throw std::runtime_error(strerror(errno));

        return CommunicationSocketType::Create(socket_fd, *(sockaddr_storage *)address->ai_addr, false);
    }
    std::future<void> listen(std::function<void(CommunicationSocket communication_socket)> after_accept,
                             int queue_size = 20) {
        return std::async(std::launch::async, [after_accept, this, queue_size]() {
            if (!is_binded)
                bind();

            ::listen(socket_fd, queue_size);
            sockaddr_storage client_address;
            socklen_t client_address_size = sizeof(client_address);
            while (true) {
                int new_socket_fd = ::accept(socket_fd, (struct sockaddr *)&client_address, &client_address_size);

                if (new_socket_fd == -1) {
                    std::cout << "Accept error: " << strerror(errno) << '\n';
                    break;
                }
                threads.emplace_back(
                    std::async(std::launch::async, after_accept,
                               std::move(CommunicationSocketType::Create(new_socket_fd, client_address, true))));
            }
        });
    }

    bool allow_reuse() {
        int enable_flag = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable_flag, sizeof(int)) < 0 ||
            setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &enable_flag, sizeof(int)) < 0) {
            return false;
        }
        return true;
    }

    static addrinfo get_default_addrinfo() {
        addrinfo result;
        memset(&result, 0, sizeof(result));
        result.ai_socktype = SOCK_STREAM;
        result.ai_family = AF_UNSPEC;
        result.ai_flags = AI_PASSIVE;

        return result;
    }

    ~ConnectionSocket() {
        // ::close(socket_fd);
        ::freeaddrinfo(address);
    }
};
}  // namespace tcp_socket
