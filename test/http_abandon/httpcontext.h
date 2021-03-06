#ifndef __HTTP_CONTEXT_H
#define __HTTP_CONTEXT_H
#include "../../net/buffer.h"
#include "httprequest.h"
class HttpContext
{
public:
	enum HttpRequestParseState
	{
		kExpectRequestLine,
		kExpectHeaders,
		kExpectBody,
		kDone,
	};
	
	HttpContext() : state_(kExpectRequestLine) {}

	bool parseRequest(netcore::Buffer* buf);

	bool parseDone() const { return state_ == kDone; }

	void reset() 
	{
		state_ = kExpectRequestLine;
		HttpRequest dummy;
		request_.swap(dummy);
	}

	const HttpRequest& request() const { return request_; }

	HttpRequest& request() {return request_;}

private:
	bool processRequestLine(const char* begin, const char* end);

private:
	HttpRequestParseState state_;
	HttpRequest request_;

};

#endif
