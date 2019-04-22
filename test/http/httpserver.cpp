#include "httpserver.h"
#include <functional>
#include "../../utils/timestamp.h"
#include "../../utils/logger.h"
using namespace netcore;

HttpServer::HttpServer(EventLoop *loop, const NetAddr &addr, const std::string &name)
	:loop_(loop),
	server_(loop, addr, name),
	httpCallback_(std::bind(&HttpServer::defalutHttpCallback, this, std::placeholders::_1, std::placeholders::_2))
{
	server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
	server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
}

HttpServer::~HttpServer()
{
	stop();
}

void HttpServer::onMessage(const ConnectionPtr &conn, Buffer &buffer)
{	
	HttpContext * http_ctx = conn->ctx_;
	bool ret = http_ctx->parse(&buffer);
	if (!ret)
	{
		conn->send("HTTP/1.1 404 Bad Request\r\n\r\n");
		conn->shutdown();
	}
	else if (http_ctx->parseDone())
	{
		httpparser::HttpResponse response;
		httpCallback_(http_ctx->message(), &response);
		conn->send(response.toStr());
		if (!response.getKeepAlive())
			conn->shutdown();
		http_ctx->reset();
	}
}

void HttpServer::onConnection(const ConnectionPtr &conn)
{
	if (conn->isConnected())
	{
		conn->ctx_ = new HttpContext();
		conn->ctx_->init(HTTP_REQUEST);
	}
	else
	{
		delete conn->ctx_;
	}
}

void HttpServer::defalutHttpCallback(const httpparser::HttpMessage *request, httpparser::HttpResponse * response)
{
	response->setStatus(HTTP_STATUS_OK);
	response->setKeepAlive(true, true);
	response->setBody(std::string("hello"), true);
}
