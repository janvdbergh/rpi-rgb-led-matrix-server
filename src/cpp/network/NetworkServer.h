#ifndef DISPLAYSERVER_NETWORK_SERVER_H
#define DISPLAYSERVER_NETWORK_SERVER_H

#include <boost/asio.hpp>
#include "PacketHandler.h"

class NetworkServer {
public:
	NetworkServer(uint16_t port, PacketHandler& packet_handler) : _port(port), _packet_handler(packet_handler) {}

	void StartServerAndBlock();

private:
	uint16_t _port;
	PacketHandler& _packet_handler;

	void HandleClientConnection(boost::asio::ip::tcp::socket &socket);

	NetworkPacketPtr read_packet(boost::asio::ip::tcp::socket &socket);
	void write_packet(boost::asio::ip::tcp::socket &socket, const NetworkPacketPtr packet);
};

#endif //DISPLAYSERVER_NETWORK_SERVER_H
