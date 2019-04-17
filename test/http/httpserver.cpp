#include "httpserver.h"
#include <functional>
#include "../../utils/timestamp.h"
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
	int fd = conn->fd();

	auto iter = ctx_.find(fd);
	if (iter == ctx_.end())
	{
		conn->shutdown();
	}
	else
	{
		HttpContext & http_ctx = iter->second;
		bool ret = http_ctx.parse(&buffer);
		if (!ret)
		{
			conn->send("HTTP/1.1 404 Bad Request\r\n\r\n");
			conn->shutdown();
		}
		else if (http_ctx.parseDone())
		{
			httpparser::HttpResponse response;
			httpCallback_(http_ctx.message(), &response);
			Buffer buf(65536);
			buf.append(response.toStr());
			conn->send(buf);
			if (!response.getKeepAlive())
				conn->shutdown();
			http_ctx.reset();
		}
	}
}

void HttpServer::onConnection(const ConnectionPtr &conn)
{
	int fd = conn->fd();
	if (conn->isConnected())
	{
		assert(ctx_.find(fd) == ctx_.end());
		if (ctx_.find(fd) != ctx_.end())
			printf("ctx_ already existed fd %d\n", fd);

		ctx_.emplace(std::make_pair(fd, HttpContext()));
		ctx_[fd].init(HTTP_REQUEST);
	}
	else
	{
		assert(ctx_.find(fd) != ctx_.end());
		if (ctx_.find(fd) != ctx_.end())
			ctx_.erase(fd);
		else
			printf("ctx_ not exsict fd %d\n", fd);
	}
}

void HttpServer::defalutHttpCallback(const httpparser::HttpMessage *request, httpparser::HttpResponse * response)
{
	response->setStatus(HTTP_STATUS_OK);
	response->setKeepAlive(true, true);
	response->setBody(std::string("hello"), true);
}