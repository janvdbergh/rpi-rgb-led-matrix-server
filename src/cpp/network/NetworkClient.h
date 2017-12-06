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

    Client &DrawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height);

    Client &DrawDigit(uint8_t position, uint8_t digit);

    Client &DrawSmallText(int16_t x, int16_t y, const std::string &text);

    Client &DrawLargeText(int16_t x, int16_t y, const std::string &text);

    Client &DefineImage(const std::string& name, boost::shared_ptr<const Image> image);

    Client &DrawImage(int16_t x, int16_t y, const std::string &name);

    Client &Show();

private:
    boost::asio::ip::tcp::socket _socket;

    void SendCommand(const boost::shared_ptr<const Command> &command);
};


#endif //DISPLAYSERVER_CLIENT_H
