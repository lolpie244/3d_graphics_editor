#pragma once

#include <alpaca/alpaca.h>

#include <glm/glm.hpp>
#include <sstream>

#include "network/communication_socket.h"
#include "network/connection_socket.h"
#include "render/opengl/picking_texture.h"
#include "utils/settings.h"

class EditorStage;

class Collaborator {
   public:
    enum Events {
        VertexMove,
        VertexAdd,

        Host_ConnectionAttempt,

        Client_Connected
    };

    struct EventData {
        Events event;
        tcp_socket::BytesType data;
    };

    typedef std::function<void(Collaborator*, const tcp_socket::BytesType& data)> EventHandler;

   public:
    Collaborator(EditorStage* stage);
    void SendVertexMoved(render::PickingTexture::Info vertex, glm::vec3 moved_to);
    void VertexMovedHandler(const tcp_socket::BytesType& data);

    template <typename T>
    void SendData(Events event, const T& data) {
        EventData event_data{.event = event};
        tcp_socket::BytesType bytes;

        alpaca::serialize(data, event_data.data);
        alpaca::serialize(event_data, bytes);
        SendBytes(bytes);
    }
    void SendData(Events event) {
        EventData event_data{.event = event};
        tcp_socket::BytesType bytes;

        alpaca::serialize(event_data, bytes);
        SendBytes(bytes);
    }

    EventData ReceiveBytes(const tcp_socket::BytesType& bytes) {
        std::error_code ec;
        return alpaca::deserialize<EventData>(bytes, ec);
    }

   protected:
    virtual void SendBytes(const tcp_socket::BytesType& data) = 0;
    virtual void ReceiveData(const EventData& data);

   protected:
    EditorStage* stage;
};

class Host : public Collaborator {
   public:
    typedef std::function<void(Host*, const tcp_socket::CommunicationSocket& socket, const tcp_socket::BytesType& data)>
        EventHandler;

   public:
    Host(EditorStage* stage);

   protected:
    void SendBytes(const tcp_socket::BytesType& data) override;
    void ReceiveData(const EventData& data, const tcp_socket::CommunicationSocket& socket);

    void NewConnection(const tcp_socket::CommunicationSocket& socket, const tcp_socket::BytesType& data);

   private:
    std::unique_ptr<tcp_socket::ConnectionSocket> socket;
    std::vector<tcp_socket::CommunicationSocket> clients_sockets;

    std::future<void> listener;
};

class Client : public Collaborator {
   public:
    Client(EditorStage* stage);

   protected:
    void SendBytes(const tcp_socket::BytesType& data) override;

   private:
    std::unique_ptr<tcp_socket::CommunicationSocket> socket;

    std::future<void> listener;
};
