#pragma once

#include <alpaca/alpaca.h>

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

        Event_Host_ConnectionAttempt,

        Event_Client_Connected
    };

    struct EventData {
        Events event;
        tcp_socket::BytesType data;

        EventData() : event(Event_InvalidEvent) {}
        EventData(Events event) : event(event) {}
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
   public:
    typedef std::function<void(Host*, tcp_socket::CommunicationSocket& socket, const tcp_socket::BytesType& data)>
        EventHandler;

   public:
    Host(EditorStage* stage);

   protected:
    virtual void SendEvent(const EventData& event);
    void ReceiveData(const EventData& data, tcp_socket::CommunicationSocket& socket);

   private:
    std::unique_ptr<tcp_socket::ConnectionSocket> socket;
    std::list<tcp_socket::CommunicationSocket> clients_sockets;

    std::future<void> listener;
};

class Client : public Collaborator {
   public:
    Client(EditorStage* stage);

   protected:
    virtual void SendEvent(const EventData& event);

   private:
    tcp_socket::CommunicationSocket socket;

    std::future<void> listener;
};
