#pragma once

#include <alpaca/alpaca.h>
#include <sys/socket.h>

#include <glm/glm.hpp>
#include <sstream>

#include "network/communication_socket.h"
#include "network/connection_socket.h"
#include "render/light.h"
#include "render/model.h"
#include "render/opengl/picking_texture.h"
#include "utils/settings.h"

class EditorStage;

class Collaborator {
   public:
    enum Events {
        Event_InvalidEvent,

        Event_VertexMove,
        Event_VertexAdd,
        Event_ModelAdd,
        Event_ModelTransform,
        Event_LightAdd,
        Event_LightTransform,

        Event_ClientConnected,
    };

    struct EventData {
        Events event;
        tcp_socket::BytesType data;

        EventData() : event(Event_InvalidEvent) {}
        EventData(Events event) : event(event) {}
        EventData(Events event, const tcp_socket::BytesType& raw_data) : event(event), data(raw_data) {}
        template <typename T>
        EventData(Events event, const T& raw_data) : event(event) {
            tcp_socket::BytesType bytes;
            alpaca::serialize(raw_data, bytes);
            this->data = bytes;
        }
    };

    typedef std::function<void(Collaborator*, const tcp_socket::BytesType& data)> EventHandler;

   public:
    Collaborator(EditorStage* stage);
    void VertexMoved(render::PickingTexture::Info vertex, glm::vec3 moved_to);
    void VertexMovedHandler(const tcp_socket::BytesType& data);

    void NewModel(render::Model* model);
    void NewModelHandler(const tcp_socket::BytesType& data);

    void ModelTransform(render::Model* model);
    void ModelTransformHandler(const tcp_socket::BytesType& data);

    void NewLight(render::Light* light);
    void NewLightHandler(const tcp_socket::BytesType& data);

    void LightTransform(render::Light* light);
    void LightTransformHandler(const tcp_socket::BytesType& data);

   protected:
    virtual void SendEvent(const EventData& event) = 0;
    virtual void ReceiveData(const EventData& data);

   protected:
    EditorStage* stage;
};

class Host : public Collaborator {
    typedef std::function<void(Host*, tcp_socket::CommunicationSocket& socket, const tcp_socket::BytesType& data)>
        EventHandler;

   public:
    Host(EditorStage* stage, sockaddr_storage address);

   protected:
    void NewClient(tcp_socket::CommunicationSocket);

    void SendEvent(const EventData& event) override;
    void ReceiveData(const EventData& data, tcp_socket::CommunicationSocket& socket);

   private:
    std::unique_ptr<tcp_socket::ConnectionSocket> socket;
    std::list<tcp_socket::CommunicationSocket> clients_sockets;

    std::future<void> listener;
};

class Client : public Collaborator {
    typedef std::function<void(Client*, const tcp_socket::BytesType& data)> EventHandler;

   public:
    Client(EditorStage* stage, sockaddr_storage address);

   protected:
    void ClientConnectedHandler(const tcp_socket::BytesType& data);
    void ReceiveData(const EventData& data) override;

    void SendEvent(const EventData& event) override;

   private:
    tcp_socket::CommunicationSocket socket;

    std::future<void> listener;
};
