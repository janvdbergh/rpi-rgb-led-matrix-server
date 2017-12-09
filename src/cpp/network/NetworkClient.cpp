//
// Created by Jan Van den Bergh on 12/11/2017.
//

#include "NetworkClient.h"
#include "NetworkError.h"

using boost::asio::ip::tcp;

Client::Client(boost::asio::io_service& io_service, const std::string& host, uint16_t port) : _socket(io_service) {
    tcp::resolver resolver(io_service);
    boost::asio::ip::basic_resolver_query<tcp> query = tcp::resolver::query(host, std::to_string(port));
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::connect(_socket, endpoint_iterator);
}

Client::~Client() {
    _socket.close();
}

Client& Client::SendCommand(const CommandPtr &command) {
    Packet packet(command);

    std::vector<boost::asio::mutable_buffer> bufs;
    boost::system::error_code error;

    uint32_t size = packet.GetSize();
    std::vector<char> data = packet.GetData();
    uint32_t crc = packet.GetCRC();

    bufs.push_back(boost::asio::buffer(&size, sizeof(size)));
    bufs.push_back(boost::asio::buffer(data.data(), size));
    bufs.push_back(boost::asio::buffer(&crc, sizeof(crc)));

    boost::asio::write(_socket, bufs, error);

    if (error) {
        boost::throw_exception(WriteError());
    }

    return *this;
}

