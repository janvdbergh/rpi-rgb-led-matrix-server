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

Client & Client::SetColor(uint8_t r, uint8_t g, uint8_t b) {
    WritePacket(PacketWriter() << COLOR << r << g << b);
    return *this;
}

Client &Client::Clear() {
    WritePacket(PacketWriter() << CLEAR);
    return *this;
}

Client &Client::DrawPixel(int16_t x, int16_t y) {
    WritePacket(PacketWriter() << PIXEL << x << y);
    return *this;
}

Client &Client::DrawRectangle(int16_t x, int16_t y, int16_t width, int16_t height) {
    WritePacket(PacketWriter() << RECTANGLE << x << y << width << height);
    return *this;
}

Client &Client::DrawDigit(uint8_t position, uint8_t digit) {
    WritePacket(PacketWriter() << DIGIT << position << digit);
    return *this;
}

Client &Client::DrawSmallText(int16_t x, int16_t y, const std::string &text) {
    WritePacket(PacketWriter() << SMALL_TEXT << x << y << text);
    return *this;
}

Client &Client::DrawLargeText(int16_t x, int16_t y, const std::string &text) {
    WritePacket(PacketWriter() << LARGE_TEXT << x << y << text);
    return *this;
}

Client &Client::Show() {
    WritePacket(PacketWriter() << SHOW);
    return *this;
}

void Client::WritePacket(const Packet &packet) {
    std::vector<boost::asio::mutable_buffer> bufs;
    boost::system::error_code error;

    uint32_t size = packet.GetSize();
    std::vector<char> data = packet.GetData();
    unsigned long crc = packet.GetCRC();
    
    bufs.push_back(boost::asio::buffer(&size, sizeof(size)));
    bufs.push_back(boost::asio::buffer(data.data(), size));
    bufs.push_back(boost::asio::buffer(&crc, sizeof(crc)));

    boost::asio::write(_socket, bufs, error);

    if (error) {
        boost::throw_exception(WriteError());
    }
}

