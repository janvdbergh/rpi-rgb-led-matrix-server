#include <utility>

#ifndef DISPLAYSERVER_NetworkError_H
#define DISPLAYSERVER_NetworkError_H

#include <string>

enum NetworkErrorCode {
	NETWORK_ERROR,
	MESSAGE_ERROR,
	CRC_ERROR
};

class NetworkError : public std::exception {
public:
	NetworkError(NetworkErrorCode code, std::string message) : _code(code), _message(std::move(message)) {}

	NetworkErrorCode get_code() const { return _code; }

	const char *what() const noexcept override { return _message.c_str(); }

private:
	NetworkErrorCode _code;
	std::string _message;
};


#endif //DISPLAYSERVER_NetworkError_H
