#pragma once

#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>
#include <string>

#include "network/network.h"

constexpr char PORT[] = "2349";
constexpr int PACKAGE_SIZE = 1000;
constexpr int CODE_LEN = 6;

struct ServerEvent {
    std::string data;

	ServerEvent() = default;
    ServerEvent(const std::string& code) { data = code.substr(0, CODE_LEN); }
};

enum Status {
    Failed,
    Client,
    Server,
};

struct ClientEvent {
    tcp_socket::BytesType address;
    Status status;

    ClientEvent() = default;

    ClientEvent(Status status, sockaddr_storage address_storage) : status(status) {
        address.resize(sizeof(address_storage));
        memcpy(address.data(), &address_storage, sizeof(address_storage));
    }
    ClientEvent(Status status) : status(status) {}

    sockaddr_storage GetAddress() {
        auto data = (sockaddr_storage*)malloc(address.size());
        memcpy(data, address.data(), address.size());

        auto result = *data;
        delete data;

        return result;
    }
};
