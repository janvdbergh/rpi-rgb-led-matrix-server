#include <sstream>
#include "NetworkError.h"

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

UnknownCommandError::UnknownCommandError(boost::endian::little_uint16_at command) : NetworkError(
        GetUnknownCommandMessage(command)) {}

InvalidSizeError::InvalidSizeError(size_t expectedSize, size_t actualSize): NetworkError(
        GetInvalidSizeMessage(expectedSize, actualSize)) {}