#include "DisplayError.h"

DisplayError::DisplayError(DisplayErrorCode code, std::string message) :
		_code(code), _message(std::move(message)) {}
