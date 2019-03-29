#include "httpserver.h"


HttpServer::HttpServer(EventLoop *loop, const NetAddr &addr, std::string &name)
	:loop_(loop),
	server_(loop, addr, name),
	httpCallback_(std::bind(&HttpServer::defalutHttpCallback, this, std::placeholders::_1, std::placeholders::_2))
{

}

HttpServer::~HttpServer()
{
	stop();
}

void HttpServer::onMessage(const ConnectionPtr &conn, Buffer &buffer)
{
	int fd = conn->fd();
	if (ctx_.find(fd) == ctx_.end())
	{
		std::cout << "HttpServer::onMessage error" << std::endl;
		conn->shutdown();
	}

	auto & http_ctx = ctx_[fd];
	bool ret = http_ctx.parseRequest(buffer);
	if (!ret)
	{
		conn->send("HTTP/1.1 404 Bad Request\r\n\r\n");
		conn->shutdown();
	}
	else if (http_ctx.parseDone())
	{
		onRequest(conn, http_ctx.request());
		http_ctx.reset();
	}
}

void HttpServer::onConnection(const ConnectionPtr &conn)
{
	int fd = conn->fd();
	if (conn->isConnected())
	{
		assert(ctx_.find(fd) == ctx_.end());
		ctx_[fd] = HttpContext();
	}
	else
	{
		assert(ctx_.find(fd) != ctx_.end());
		ctx_.erase(fd);
	}
}

void HttpServer::onRequest(const ConnectionPtr &conn, const HttpRequest &request)
{
	HttpResponse response;
	httpCallback_(request, &response);
	conn->send(response.getBuf());
	conn->shutdown();
}
	
void HttpServer::defalutHttpCallback(const HttpRequest &request, HttpResponse *response)
{
	
}