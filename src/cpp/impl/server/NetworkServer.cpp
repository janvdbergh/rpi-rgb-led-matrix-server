#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <regex>

#include <server/NetworkServer.h>
#include <common/NetworkError.h>

using boost::asio::ip::tcp;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void NetworkServer::StartServerAndBlock() {
	try {
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), _port));

		std::cout << "Opening server on port " << _port << std::endl;

		for (;;) {
			tcp::socket socket(io_service);
			acceptor.accept(socket);

			HandleClientConnection(socket);
		}
	} catch (std::exception &e) {
		std::cerr << "TCP server error: " << e.what() << std::endl;
	}

	std::cout << "Server stopped" << std::endl;
}

#pragma clang diagnostic pop

void NetworkServer::HandleClientConnection(tcp::socket &socket) {
	std::cout << "Connection open to " << socket.remote_endpoint() << std::endl;

	try {
		while (socket.is_open()) {
			NetworkPacketPtr request = read_packet(socket);
			NetworkPacketPtr response = _packet_handler.handle_packet(request);
			write_packet(socket, response);
		}
	} catch (NetworkError &e) {
		std::cerr << "Network error: " << e.get_code() << " (" << e.what() << ")" << std::endl;
	}

	std::cout << "Connection closed to " << socket.remote_endpoint() << std::endl;
	socket.close();
}

NetworkPacketPtr NetworkServer::read_packet(tcp::socket &socket) {
	std::vector<boost::asio::mutable_buffer> bufs;
	boost::system::error_code error;

	uint32_t data_length;
	bufs.clear();
	bufs.push_back(boost::asio::buffer(&data_length, sizeof(data_length)));
	boost::asio::read(socket, bufs, error);
	if (error) {
		boost::throw_exception(NetworkError(NETWORK_ERROR, "Read error"));
	}
	if (data_length > MAX_PACKET_SIZE) {
		boost::throw_exception(NetworkError(MESSAGE_ERROR, "Packet too large"));
	}

	std::vector<uint8_t> data(data_length);
	uint32_t actual_crc;
	bufs.clear();
	bufs.push_back(boost::asio::buffer(&data.front(), data_length));
	bufs.push_back(boost::asio::buffer(&actual_crc, sizeof(actual_crc)));
	boost::asio::read(socket, bufs, error);
	if (error) {
		boost::throw_exception(NetworkError(NETWORK_ERROR, "Read error"));
	}

	NetworkPacketPtr result(new NetworkPacket(data));
	if (result->get_crc() != actual_crc) {
		boost::throw_exception(NetworkError(CRC_ERROR, "Invalid CRC"));
	}

	return result;
}

void NetworkServer::write_packet(boost::asio::ip::tcp::socket &socket, const NetworkPacketPtr packet) {
	std::vector<boost::asio::mutable_buffer> bufs;
	boost::system::error_code error;

	auto size = static_cast<uint32_t>(packet->get_data().size());
	std::vector<uint8_t> data = packet->get_data();
	uint32_t crc = packet->get_crc();

	bufs.push_back(boost::asio::buffer(&size, sizeof(size)));
	bufs.push_back(boost::asio::buffer(data, size));
	bufs.push_back(boost::asio::buffer(&crc, sizeof(crc)));

	boost::asio::write(socket, bufs, error);

	if (error) {
		boost::throw_exception(NetworkError(NETWORK_ERROR, "Write error"));
	}
}

