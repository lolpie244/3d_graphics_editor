#pragma once

#include <glm/glm.hpp>
#include <sstream>

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

    typedef std::function<void(Collaborator*, std::stringstream& data)> EventHandler;

   public:
	Collaborator(EditorStage* stage);
    void SendVertexMoved(render::PickingTexture::Info vertex, glm::vec3 moved_to);
    void VertexMovedHandler(std::stringstream& data);

   protected:
    virtual void SendData(const std::string& data) = 0;
    virtual void ReceiveData(std::stringstream& data);

   protected:
    EditorStage* stage;
};

class Host : public Collaborator {
   public:
    typedef std::function<void(Host*, const tcp_socket::CommunicationSocket& socket, std::stringstream& data)>
        EventHandler;

   public:
    Host(EditorStage* stage);

   protected:
    void SendData(const std::string& data) override;
    void ReceiveData(std::stringstream& message, const tcp_socket::CommunicationSocket& socket);

    void NewConnection(const tcp_socket::CommunicationSocket& socket, std::stringstream& data);

   private:
    std::unique_ptr<tcp_socket::ConnectionSocket> socket;
    std::vector<tcp_socket::CommunicationSocket> clients_sockets;

    std::future<void> listener;
};

class Client : public Collaborator {
   public:
    Client(EditorStage* stage);

   protected:
    void SendData(const std::string& data) override;

   private:
    std::unique_ptr<tcp_socket::CommunicationSocket> socket;

	std::future<void> listener;
};
