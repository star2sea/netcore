#include "httpcontext.h"
#include <algorithm>
using namespace netcore;

HttpContext::HttpContext()
	:parseError_(false),
	parseDone_(false)
{
	parser_.parser_init(&request_, HTTP_REQUEST, [&](bool ret) {
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
	return parseError_;
}