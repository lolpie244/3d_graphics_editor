#pragma once

#include <glm/glm.hpp>
#include <sstream>

#include "network/connection_socket.h"
#include "render/opengl/picking_texture.h"
#include "utils/settings.h"

class Collaborator {
   public:
    enum Events {
        VertexMove,
        VertexAdd,
    };

    typedef std::function<void(Collaborator*, std::stringstream& data)> EventHandler;

   public:
    void SendVertexMoved(render::PickingTexture::Info vertex, glm::vec3 moved_to);
    void VertexMovedHandler(std::stringstream& data);

   protected:
    virtual void SendData(const std::string& data) = 0;
    virtual void ReceiveData(std::stringstream& data);
};

class Host : public Collaborator {
   public:
    enum Events {
        ConnectionAttempt,
    };

    typedef std::function<void(Host*, const tcp_socket::CommunicationSocket& socket, std::stringstream& data)> EventHandler;

   public:
    Host();

   protected:
    void SendData(const std::string& data) override;
    void ReceiveData(std::stringstream& message, const tcp_socket::CommunicationSocket& socket);

   private:
    std::unique_ptr<tcp_socket::ConnectionSocket> socket;
    std::vector<std::unique_ptr<tcp_socket::CommunicationSocket>> clients_sockets;
};

class Client : public Collaborator {
   public:
    enum Events {
        Connected,
    };

   public:
    Client();

   private:
    std::unique_ptr<tcp_socket::CommunicationSocket> socket;
};
