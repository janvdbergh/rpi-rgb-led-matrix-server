#ifndef DISPLAYSERVER_PACKETHANDLER_H
#define DISPLAYSERVER_PACKETHANDLER_H


#include <common/NetworkPacket.h>

class PacketHandler {
public:
	virtual const NetworkPacketPtr handle_packet(NetworkPacketPtr request) = 0;
};


#endif //DISPLAYSERVER_PACKETHANDLER_H
