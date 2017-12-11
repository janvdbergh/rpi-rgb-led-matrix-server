#ifndef DISPLAYSERVER_RESPONSE_H
#define DISPLAYSERVER_RESPONSE_H


#include <string>
#include <boost/shared_ptr.hpp>

enum ResponseCode {
	NETWORK_ERROR = -99,
	CRC_ERROR = -98,
	MESSAGE_ERROR = -97,
	UNKNOWN_COMMAND = -10,
	UNKNOWN_IMAGE = -2,
	UNKNOWN_ANIMATION = -1,

	OK = 0
};

class Response {
public:
	explicit Response(ResponseCode responseCode, std::string detailMessage = "") :
			_responseCode(responseCode), _detailMessage(std::move(detailMessage)) {}

	ResponseCode GetResponseCode() const { return _responseCode; }

	const std::string &GetDetailMessage() const { return _detailMessage; }

	bool IsError() { return _responseCode != OK; }

private:
	ResponseCode _responseCode;
	std::string _detailMessage;
};
typedef boost::shared_ptr<Response> ResponsePtr;


#endif //DISPLAYSERVER_RESPONSE_H
