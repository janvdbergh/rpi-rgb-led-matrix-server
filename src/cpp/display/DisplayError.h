#include <utility>

#ifndef DISPLAYSERVER_COMMANDERROR_H
#define DISPLAYSERVER_COMMANDERROR_H

#include <string>

enum DisplayErrorCode {
	LAYER_ALREADY_EXISTS,
	LAYER_NOT_FOUND,
	IMAGE_ALREADY_EXISTS,
};

class DisplayError : public std::exception {
public:
	DisplayError(DisplayErrorCode code, std::string message);

	DisplayErrorCode get_code() const { return _code; }

	const char *what() const noexcept override { return _message.c_str(); }

private:
	DisplayErrorCode _code;
	std::string _message;
};


#endif //DISPLAYSERVER_COMMANDERROR_H
