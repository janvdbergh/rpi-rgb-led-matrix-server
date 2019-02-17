#include <utility>

#ifndef DISPLAYSERVER_PACKET_H
#define DISPLAYSERVER_PACKET_H

#include <vector>
#include <string>
#include <memory>

const uint32_t MAX_PACKET_SIZE = 8192 * 8;

class NetworkPacket {
public:
	explicit NetworkPacket(std::vector<uint8_t> _data) : _data(std::move(_data)) {}

	const std::vector<uint8_t> &get_data() const { return _data; }

	uint32_t get_crc() const;

protected:
	const std::vector<uint8_t > _data;
};

typedef std::shared_ptr<NetworkPacket> NetworkPacketPtr;

#endif //DISPLAYSERVER_PACKET_H
