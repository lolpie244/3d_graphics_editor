#include <future>
#include <stdexcept>
#include <string>

#include "editor/editor.h"
#include "network.h"
#include "network/communication_socket.h"
#include "network/connection_socket.h"
#include "utils/settings.h"

Client::Client(EditorStage* stage, sockaddr_storage address) : Collaborator(stage) {
    tcp_socket::ConnectionSocket host_socket = tcp_socket::ConnectionSocket(address);

    bool run = true;
    auto try_connect = std::async(std::launch::async, [this, &host_socket, &run]() {
        while (run) {
            try {
                this->socket = host_socket.connect();
                break;
            } catch (...) {}
        }
    });

    if (try_connect.wait_for(settings::WAIT_FOR_CONNECTION) != std::future_status::ready) {
        run = false;
        throw std::runtime_error("Can't connect");
    }
    std::cout << "CONNECTED\n";

    socket->start_recieve_loop<EventData>([this](const EventData& event) { ReceiveData(event); });
}

void Client::SendEvent(const EventData& event) { socket->send(event); }

void Client::ClientConnectedHandler(const tcp_socket::BytesType& data) {
    stage->ScheduleWork([data, this]() { stage->LoadScene(data, false); });
}

void Client::ReceiveData(const EventData& event) {
    static const std::unordered_map<unsigned int, EventHandler> events{
        {Event_ClientConnected, &Client::ClientConnectedHandler},
    };

    if (events.contains(event.event))
        events.at(event.event)(this, event.data);
    else {
        Collaborator::ReceiveData(event);
    }
}
