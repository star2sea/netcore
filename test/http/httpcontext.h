#ifndef __HTTP_CONTEXT_H
#define __HTTP_CONTEXT_H
#include "../../net/buffer.h"
#include "http_parser/httpparser.h"
class HttpContext
{
public:
	HttpContext();

	bool parse(netcore::Buffer* buf);

	void reset() { request_.reset(); parseError_ = false; parseDone_ = false; }

	bool parseDone() const { return parseDone_; }

	httpparser::HttpRequest request() const { return request_; }

private:
	httpparser::HttpRequest request_;
	httpparser::HttpParser parser_;
	bool parseError_;
	bool parseDone_;
};

#endif
