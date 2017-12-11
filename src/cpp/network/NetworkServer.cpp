#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <regex>

#include "../display/DisplayError.h"
#include "NetworkServer.h"

using boost::asio::ip::tcp;

void Server::StartServerAndBlock() {
	try {
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), _port));

		std::cout << "Opening server on port " << _port << std::endl;

		for (;;) {
			tcp::socket socket(io_service);
			acceptor.accept(socket);

			HandleClientConnection(socket);
			_display->Reset();
		}
	} catch (std::exception &e) {
		std::cerr << "TCP server error: " << e.what() << std::endl;
	}

	std::cout << "Server stopped" << std::endl;
}

void Server::HandleClientConnection(tcp::socket &socket) {
	std::cout << "Connection open to " << socket.remote_endpoint() << std::endl;

	try {
		while (socket.is_open()) {
			Packet commandPacket(socket);
			HandlePacket(commandPacket);

			ResponsePtr response = ResponsePtr(new Response(ResponseCode::OK, "OK"));
			Packet(response).Send(socket);
		}
	} catch (DisplayError &e) {
		std::cerr << "Error: " << e.responseCode() << " (" << e.what() << ")" << std::endl;
		ResponsePtr response = ResponsePtr(new Response(e.responseCode(), e.what()));

		try {
			Packet(response).Send(socket);
		} catch (DisplayError &e) {
			std::cerr << "Error while sending error: " << e.responseCode() << " - " << e.what() << std::endl;
		}
	}

	std::cout << "Connection closed to " << socket.remote_endpoint() << std::endl;
	socket.close();
}

void Server::HandlePacket(const Packet &packet) {
	_display->ExecuteCommand(packet.GetCommand());
}

