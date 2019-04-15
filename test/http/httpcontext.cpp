#include "httpcontext.h"
#include <algorithm>
#include "../../utils/logger.h"
using namespace netcore;

void HttpContext::init(http_parser_type t)
{
	if (t == HTTP_REQUEST)
	{
		message_ = new httpparser::HttpRequest();
	}
	else if (t == HTTP_RESPONSE)
	{
		message_ = new httpparser::HttpResponse();
	}
	else 
	{
		LOG_ERROR << "HttpContext init error, http_parser_type = " << t;
		return;
	}
	parser_.parser_init(message_, HTTP_REQUEST, [&](bool ret) {
		if (ret)
		{
			parseDone_ = true;
		}
		else
		{
			parseError_ = true;
		}
	});
}

bool HttpContext::parse(Buffer* buf)
{
	size_t parsed = parser_.parse(buf->readBegin(), buf->readAvailable());
	buf->consume(parsed);
	return !parseError_;
}