#ifndef __HTTP_RESPONSE_H
#define __HTTP_RESPONSE_H
#include "../../net/buffer.h"
#include <map>
#include <string>
class HttpResponse
{
public:
	enum HttpStatusCode
	{
		kUnknown,
		k200Ok = 200,
		k301MovedPermanently = 301,
		k400BadRequest = 400,
		k404NotFound = 404,
	};

	HttpResponse() : statusCode_(kUnknown), connectionClose_(false) {}

	void setStatusCode(HttpStatusCode code) { statusCode_ = code; }

	void setStatusMessage(const std::string & msg) { statusMessage_ = msg; }

	void setContentType(const std::string & contentType) { addHeader("Content-Type", contentType); }

	void setBody(const std::string & body) { body_ = body; }

	void addHeader(const std::string &k, const std::string &v) { headers_[k] = v; }

	void appendToBuffer(netcore::Buffer *buffer);

	void setConnectionClose(bool on) { connectionClose_ = on; }

	bool connectionClose()const { return connectionClose_; }

private:
	std::map<std::string, std::string> headers_;
	HttpStatusCode statusCode_;
	bool connectionClose_;
	std::string statusMessage_;
	std::string body_;
};


#endif