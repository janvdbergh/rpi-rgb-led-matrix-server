#ifndef DISPLAYSERVER_ERRORS_H
#define DISPLAYSERVER_ERRORS_H

#include <boost/endian/arithmetic.hpp>
#include <string>

class NetworkError : public std::exception {
public:
	explicit NetworkError(std::string message) : _message(std::move(message)) {}

	const char *what() { return _message.c_str(); }

private:
	const std::string _message;
};

class ReadError : public NetworkError {
public:
	ReadError() : NetworkError("Read error") {}

public:
};

class WriteError : public NetworkError {
public:
	WriteError() : NetworkError("Read error") {}

public:
};

class CrcError : public NetworkError {
public:
	CrcError() : NetworkError("CRC error") {}
};

class UnknownCommandError : public NetworkError {
public:
	explicit UnknownCommandError(boost::endian::little_uint16_at command);
};

class PacketTooLargeError : public NetworkError {
public:
	PacketTooLargeError() : NetworkError("Packet too large") {}
};

class IndexOutOfRangeError : public NetworkError {
public:
	IndexOutOfRangeError() : NetworkError("Index out of range") {}
};

class InvalidSizeError : public NetworkError {
public:
	InvalidSizeError(size_t expectedSize, size_t actualSize);
};

#endif //DISPLAYSERVER_ERRORS_H
