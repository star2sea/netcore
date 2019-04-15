#ifndef __HTTP_CONTEXT_H
#define __HTTP_CONTEXT_H
#include "../../net/buffer.h"
#include "http_parser/httpparser.h"
class HttpContext
{
public:
	HttpContext(httpparser::HttpMessage * msg);
	~HttpContext() { delete message_; }

	bool parse(netcore::Buffer* buf);

	void reset() { message_->reset(); parseError_ = false; parseDone_ = false; }

	bool parseDone() const { return parseDone_; }

	httpparser::HttpMessage *message() const { return message_; }

private:
	httpparser::HttpMessage *message_;
	httpparser::HttpParser parser_;
	bool parseError_;
	bool parseDone_;
};

#endif
