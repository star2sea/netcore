#include "httpresponse.h"
using namespace netcore;

void HttpResponse::appendToBuffer(Buffer *buffer)
{
	char buf[32];
	snprintf(buf, sizeof buf, "HTTP/1.1 %d %s", statusCode_, statusMessage_.c_str());
	buffer->append(buf);
	buffer->append("\r\n");

	if (connectionClose())
		buffer->append("Connection: close\r\n");
	else
	{
		snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", body_.size());
		buffer->append(buf);
		buffer->append("Connection: Keep-Alive\r\n");
	}
		
	for (const auto& header : headers_)
	{
		buffer->append(header.first);
		buffer->append(": ");
		buffer->append(header.second);
		buffer->append("\r\n");
	}

	buffer->append("\r\n");
	buffer->append(body_);
}