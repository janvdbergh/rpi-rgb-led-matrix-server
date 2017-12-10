#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <regex>
#include <boost/endian/arithmetic.hpp>
#include <boost/crc.hpp>

#include "ServerError.h"
#include "NetworkServer.h"

using boost::asio::ip::tcp;

void Server::StartServerAndBlock() {
	try {
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), _port));

		std::cout << "Opening server on port " << _port << std::endl;

		for (;;) {
			tcp::socket socket(io_service);
			acceptor.accept(socket);

			this->HandleClientConnection(socket);
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
			uint32_t size = ReadDataLength(socket);
			Packet packet = ReadData(socket, size);

			HandlePacket(packet);
		}
	} catch (ServerError &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	std::cout << "Connection closed to " << socket.remote_endpoint() << std::endl;
	socket.close();
}

uint32_t Server::ReadDataLength(tcp::socket &socket) {
	std::vector<boost::asio::mutable_buffer> bufs;
	boost::system::error_code error;
	u_int32_t data_length;

	bufs.push_back(boost::asio::buffer(&data_length, sizeof(data_length)));
	boost::asio::read(socket, bufs, error);
	if (error) {
		boost::throw_exception(ReadError());
	}

	if (data_length > MAX_PACKET_SIZE) {
		boost::throw_exception(PacketTooLargeError());
	}

	return data_length;
}

Packet Server::ReadData(tcp::socket &socket, uint32_t data_length) {
	std::vector<boost::asio::mutable_buffer> bufs;
	boost::system::error_code error;
	std::vector<char> data(data_length);
	uint32_t expectedCrc;

	bufs.clear();
	bufs.push_back(boost::asio::buffer(&data.front(), data_length));
	bufs.push_back(boost::asio::buffer(&expectedCrc, sizeof(expectedCrc)));
	boost::asio::read(socket, bufs, error);
	if (error) {
		boost::throw_exception(ReadError());
	}

	Packet packet(data);
	if (packet.GetCRC() != expectedCrc) {
		boost::throw_exception(CrcError());
	}

	return Packet(data);
}

void Server::HandlePacket(Packet packet) {
	_display->ExecuteCommand(packet.GetCommand());
}
