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
    SendCommand(boost::shared_ptr<const Command>(new ColorCommand(r, g, b)));
    return *this;
}

Client &Client::Clear() {
    SendCommand(boost::shared_ptr<const Command>(new ClearCommand()));
    return *this;
}

Client &Client::DrawPixel(int16_t x, int16_t y) {
    SendCommand(boost::shared_ptr<const Command>(new PixelCommand(x, y)));
    return *this;
}

Client &Client::DrawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height) {
    SendCommand(boost::shared_ptr<const Command>(new RectangleCommand(x, y, width, height)));
    return *this;
}

Client &Client::DrawDigit(uint8_t position, uint8_t digit) {
    SendCommand(boost::shared_ptr<const Command>(new DigitCommand(position, digit)));
    return *this;
}

Client &Client::DrawSmallText(int16_t x, int16_t y, const std::string &text) {
    SendCommand(boost::shared_ptr<const Command>(new SmallTextCommand(x, y, text)));
    return *this;
}

Client &Client::DrawLargeText(int16_t x, int16_t y, const std::string &text) {
    SendCommand(boost::shared_ptr<const Command>(new LargeTextCommand(x, y, text)));
    return *this;
}

Client &Client::DefineImage(const std::string &name, boost::shared_ptr<const Image> image) {
    SendCommand(boost::shared_ptr<const Command>(new DefineImageCommand(name, image)));
    return *this;
}

Client &Client::DrawImage(int16_t x, int16_t y, const std::string &name) {
    SendCommand(boost::shared_ptr<const Command>(new ImageCommand(x, y, name)));
    return *this;
}

Client &Client::Show() {
    SendCommand(boost::shared_ptr<const Command>(new ShowCommand()));
    return *this;
}

void Client::SendCommand(const boost::shared_ptr<const Command> &command) {
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
}

