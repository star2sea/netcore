#ifndef __HTTP_CONTEXT_H
#define __HTTP_CONTEXT_H
#include "../../buffer.h"
#include "http_parser/httpparser.h"
class HttpContext
{
public:
	HttpContext() : message_(nullptr), parseError_(false), parseDone_(false) {}

	~HttpContext() { if (message_) delete message_; }

	void init(http_parser_type t);

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
