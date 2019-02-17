#include <utility>

#ifndef DISPLAYSERVER_NETWORKPACKETPARSER_H
#define DISPLAYSERVER_NETWORKPACKETPARSER_H

#include <common/NetworkPacket.h>
#include <common/Point.h>
#include <common/Pixel.h>
#include <common/Image.h>

class NetworkPacketParser {
public:
	NetworkPacketParser(NetworkPacketPtr networkPacket) : _networkPacket(std::move(networkPacket)) {}

	uint8_t read_uint8();

	int16_t read_int16();

	uint16_t read_uint16();

	Point read_point();

	Pixel read_pixel();

	std::string read_string();

	ImagePtr read_image();

	void ensure_at_end();

private:
	NetworkPacketPtr _networkPacket;
	uint16_t _position;
};


#endif //DISPLAYSERVER_NETWORKPACKETPARSER_H
