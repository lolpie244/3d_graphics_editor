#pragma once

#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>
#include <string>

#include "network/network.h"

namespace tcp_hole {

constexpr int MAX_CODE_LEN = 10;
constexpr int CLIENT_PACKAGE_SIZE = 100;

inline std::string GenerateCode(int CODE_LEN) {
	auto randchar = []() -> char {
		static const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(CODE_LEN, 0);
	std::generate_n(str.begin(), CODE_LEN, randchar);
	return str;
}

struct ServerEvent {
    std::string data;

    ServerEvent() = default;
    ServerEvent(const std::string& code) : data(code) {}

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
}  // namespace tcp_hole
