#ifndef DISPLAYSERVER_CLIENT_H
#define DISPLAYSERVER_CLIENT_H

#include <boost/asio.hpp>

#include <string>
#include "NetworkPacket.h"

class Client {
public:
    Client(boost::asio::io_service& io_service, const std::string& host, uint16_t port);

    ~Client();

    Client &SetColor(uint8_t r, uint8_t g, uint8_t b);

    Client &Clear();

    Client &DrawPixel(int16_t x, int16_t y);

    Client &DrawRectangle(int16_t x, int16_t y, int16_t width, int16_t height);

    Client &DrawDigit(uint8_t position, uint8_t digit);

    Client &DrawSmallText(int16_t x, int16_t y, const std::string &text);

    Client &DrawLargeText(int16_t x, int16_t y, const std::string &text);

    Client &Show();

private:
    boost::asio::ip::tcp::socket _socket;

    void WritePacket(const Packet &packet);
};


#endif //DISPLAYSERVER_CLIENT_H
