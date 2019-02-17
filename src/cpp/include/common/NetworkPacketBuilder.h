#ifndef DISPLAYSERVER_NETWORKPACKETBUILDER_H
#define DISPLAYSERVER_NETWORKPACKETBUILDER_H

#include <common/NetworkPacket.h>
#include <common/Point.h>
#include <common/Pixel.h>
#include <common/Image.h>

class NetworkPacketBuilder {
public:
	NetworkPacketBuilder &add_uint8(uint8_t value);

	NetworkPacketBuilder &add_int16(int16_t value);

	NetworkPacketBuilder &add_uint16(uint16_t value);

	NetworkPacketBuilder &add_point(Point point);

	NetworkPacketBuilder &add_pixel(Pixel pixel);

	NetworkPacketBuilder &add_string(const std::string &str);

	NetworkPacketBuilder &add_image(ImagePtr image);

	NetworkPacketPtr build() const;

private:
	std::vector<uint8_t> _data;
};


#endif //DISPLAYSERVER_NETWORKPACKETBUILDER_H
