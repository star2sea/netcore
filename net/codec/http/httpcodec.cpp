#include "httpcodec.h"
#include "../../connection.h"

using namespace netcore;

HttpCodec::HttpCodec(http_parser_type t)
{
	ctx_.init(t);
}

void HttpCodec::onMessage(const ConnectionPtr &conn, Buffer &buffer)
{
	bool ret = ctx_.parse(&buffer);
	if (!ret)
	{
		conn->send("HTTP/1.1 404 Bad Request\r\n\r\n");
		conn->shutdown();
	}
	else if (ctx_.parseDone())
	{
		httpparser::HttpResponse response;
		httpCallback_(conn, ctx_.message(), &response);
		conn->send(response.toStr());
		if (!response.getKeepAlive())
			conn->shutdown();
		ctx_.reset();
	}
}

void HttpCodec::defalutHttpCallback(const ConnectionPtr &conn, const httpparser::HttpMessage *msg, httpparser::HttpResponse *rsp)
{
	rsp->setStatus(HTTP_STATUS_OK);
	rsp->setKeepAlive(true, true);
}
