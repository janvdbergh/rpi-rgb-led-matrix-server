#ifndef DISPLAYSERVER_CLIENT_H
#define DISPLAYSERVER_CLIENT_H

#include <boost/asio.hpp>

#include <string>
#include "NetworkPacket.h"

class Client {
public:
	Client(boost::asio::io_service &io_service, const std::string &host, uint16_t port);

	~Client();

	Client &SendCommand(const CommandPtr &command);

private:
	boost::asio::ip::tcp::socket _socket;
};


#endif //DISPLAYSERVER_CLIENT_H
