#include <sstream>
#include "DisplayError.h"

std::string GetUnknownCommandMessage(boost::endian::little_uint16_at command) {
	std::ostringstream stringStream;
	stringStream << "Unknown command " << command;
	return stringStream.str();
}

std::string GetInvalidSizeMessage(size_t expectedSize, size_t actualSize) {
	std::ostringstream stringStream;
	stringStream << "Invalid packet size. Expected " << expectedSize << ", but got " << actualSize;
	return stringStream.str();
}
