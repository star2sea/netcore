#ifndef __HTTP_PARSER_H
#define __HTTP_PARSER_H

#include "parser/http_parser.h"
#include "httpmessage.h"
#include <string>
#include <functional>

namespace httpparser
{
#define HTTP_CB(func) \
	static int on_##func(http_parser *p){ \
		HttpParser *parser = (HttpParser *)p->data; \
		return parser->func##_cb();} \
	int func##_cb();

#define HTTP_DATA_CB(func) \
	static int on_##func(http_parser *p, const char *buf, size_t len){ \
		HttpParser *parser = (HttpParser *)p->data; \
		return parser->func##_cb(buf, len);} \
	int func##_cb(const char *buf, size_t len);

class HttpParser
{
public:
	typedef std::function<void(bool)> ParseDoneCallback;
	void parser_init(HttpMessage *msg, enum http_parser_type type, const ParseDoneCallback & cb);
	size_t parse(const char *buf, size_t len);
private:
	HTTP_CB(message_begin);
	HTTP_CB(headers_complete);
	HTTP_CB(message_complete);

	HTTP_DATA_CB(header_field);
	HTTP_DATA_CB(header_value);
	HTTP_DATA_CB(request_url);
	HTTP_DATA_CB(response_status);
	HTTP_DATA_CB(body);
	
private:
	std::string header_;
	std::string value_;

	HttpMessage *message_;
	ParseDoneCallback parseDoneCallback_;

	http_parser parser_;
	http_parser_settings settings_;
};
}

#endif