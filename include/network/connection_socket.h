#pragma once

#include <netdb.h>

#include <functional>
#include <memory>
#include <string>
#include <future>
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
    ConnectionSocket(const char *host, const char *port, const addrinfo &address);
    ConnectionSocket(const char *port);

    void bind();
    CommunicationSocket connect();
    void listen(std::function<void(CommunicationSocket communication_socket)> after_accept, int queue_size = 20);

    static addrinfo get_default_addrinfo();

    ~ConnectionSocket();
};
}  // namespace tcp_socket
