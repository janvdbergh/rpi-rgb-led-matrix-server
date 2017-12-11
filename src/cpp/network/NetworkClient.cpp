#include "NetworkClient.h"
#include "../display/DisplayError.h"

using boost::asio::ip::tcp;

Client::Client(boost::asio::io_service &io_service, const std::string &host, uint16_t port) : _socket(io_service) {
	tcp::resolver resolver(io_service);
	boost::asio::ip::basic_resolver_query<tcp> query = tcp::resolver::query(host, std::to_string(port));
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::connect(_socket, endpoint_iterator);
}

Client::~Client() {
	_socket.close();
}

Client &Client::SendCommand(const CommandPtr &command) {
	Packet(command).Send(_socket);

	Packet responsePacket(_socket);
	ResponsePtr response = responsePacket.GetResponse();
	if (response->IsError()) {
		boost::throw_exception(DisplayError(response->GetResponseCode(), response->GetDetailMessage()));
	}

	return *this;
}