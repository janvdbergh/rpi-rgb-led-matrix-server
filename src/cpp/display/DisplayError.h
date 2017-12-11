#ifndef DISPLAYSERVER_ERRORS_H
#define DISPLAYSERVER_ERRORS_H

#include <boost/endian/arithmetic.hpp>
#include <string>
#include "Response.h"

class DisplayError : public std::exception {
public:
	explicit DisplayError(ResponseCode responseCode, std::string message) : _responseCode(responseCode), _message(std::move(message)) {}

	const char *what() const _NOEXCEPT override { return _message.c_str(); }
	const ResponseCode responseCode() const { return _responseCode; }

private:
	std::string _message;
	ResponseCode _responseCode;
};

#endif //DISPLAYSERVER_ERRORS_H
