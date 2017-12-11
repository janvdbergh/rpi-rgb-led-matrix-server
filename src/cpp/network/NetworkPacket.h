#ifndef DISPLAYSERVER_PACKET_H
#define DISPLAYSERVER_PACKET_H

#include <vector>
#include <string>
#include "../display/Image.h"
#include "../display/Command.h"
#include "../display/Response.h"

const uint32_t MAX_PACKET_SIZE = 8192;

class Packet {
public:
	explicit Packet(const CommandPtr &command);

	explicit Packet(const ResponsePtr &response);

	explicit Packet(boost::asio::ip::tcp::socket& socket);

	uint32_t GetSize() const { return static_cast<uint32_t>(_data.size()); }

	const std::vector<char> &GetData() const { return _data; }

	uint32_t GetCRC() const;

	CommandPtr GetCommand() const;

	ResponsePtr GetResponse() const;

	void Send(boost::asio::ip::tcp::socket& socket);

private:
	std::vector<char> _data;
};

#endif //DISPLAYSERVER_PACKET_H
