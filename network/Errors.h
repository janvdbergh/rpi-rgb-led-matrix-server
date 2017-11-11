#ifndef DISPLAYSERVER_ERRORS_H
#define DISPLAYSERVER_ERRORS_H

#include <boost/endian/arithmetic.hpp>
#include <string>

class ServerError : public std::exception {
public:
    explicit ServerError(std::string message) : _message(std::move(message)) {}

    const char *what() { return _message.c_str(); }

private:
    const std::string _message;
};

class ReadError : public ServerError {
public:
    ReadError() : ServerError("Read error") {}

public:
};

class CrcError : public ServerError {
public:
    CrcError() : ServerError("CRC error") {}
};

class UnknownCommandError : public ServerError {
public:
    explicit UnknownCommandError(boost::endian::little_uint16_at command);
};

class PacketTooLargeError : public ServerError {
public:
    PacketTooLargeError() : ServerError("Packet too large") {}
};

class IndexOutOfRangeError : public ServerError {
public:
    IndexOutOfRangeError() : ServerError("Index out of range") {}
};

class InvalidSizeError : public ServerError {
public:
    InvalidSizeError(size_t expectedSize, size_t actualSize);
};

#endif //DISPLAYSERVER_ERRORS_H
