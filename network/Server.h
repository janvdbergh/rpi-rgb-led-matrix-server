#ifndef DISPLAYSERVER_NETWORK_SERVER_H
#define DISPLAYSERVER_NETWORK_SERVER_H

#include <boost/asio.hpp>
#include <utility>
#include "../displays/Display.h"
#include "Packet.h"

class Server {
public:
    Server(boost::shared_ptr<Display> &display, uint16_t port) : _display(display), _port(port) {}

    void StartServerAndBlock();

private:
    boost::shared_ptr<Display> _display;
    uint16_t _port;

    void HandleClientConnection(boost::asio::ip::tcp::socket &socket);

    uint32_t ReadDataLength(boost::asio::ip::tcp::socket &socket);

    Packet ReadData(boost::asio::ip::tcp::socket &socket, uint32_t data_length);

    void HandlePacket(Packet packet);
};

#endif //DISPLAYSERVER_NETWORK_SERVER_H
