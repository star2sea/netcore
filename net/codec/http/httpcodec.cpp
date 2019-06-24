#include "httpcodec.h"
#include "../../connection.h"
#include "http_parser/httpparser.h"

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
		httpparser::HttpResponse response;
		response.setStatus(HTTP_STATUS_NOT_FOUND);
		response.setKeepAlive(false, true);
		response.setBody(std::string(""), true);
		conn->send(response.toStr());
		conn->shutdown();
	}
	else if (ctx_.parseDone())
	{
		httpparser::HttpResponse response;
		httpCallback_(conn, ctx_.message(), &response);
		conn->send(response.toStr());
		if (!response.getKeepAlive())
		{
			conn->shutdown();
		}
		ctx_.reset();
	}
}

void HttpCodec::defalutHttpCallback(const ConnectionPtr &conn, const httpparser::HttpMessage *msg, httpparser::HttpResponse *rsp)
{
	rsp->setStatus(HTTP_STATUS_OK);
	rsp->setKeepAlive(false, true);
}
