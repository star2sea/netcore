#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H
#include "http_parser/httpparser.h"
#include "httpcontext.h"
#include "../../utils/noncopyable.h"
#include "../../net/eventloop.h"
#include "../../net/server.h"
#include "../../net/callback.h"
#include "../../net/buffer.h"
#include "../../net/connection.h"

class HttpServer : netcore::NonCopyable
{
public:
	typedef std::function<void(const httpparser::HttpMessage *, httpparser::HttpResponse *)> HttpCallback;

	HttpServer(netcore::EventLoop *loop, const netcore::NetAddr &addr, const std::string &name);
	~HttpServer();

	void start(int threadnum = 0) { server_.start(threadnum); }
	void stop() { server_.stop(); }

	void setHttpCallback(const HttpCallback & cb) { httpCallback_ = cb; }

private:
	void onMessage(const netcore::ConnectionPtr &conn, netcore::Buffer &buffer);
	void onConnection(const netcore::ConnectionPtr &conn);
	void defalutHttpCallback(const httpparser::HttpMessage *, httpparser::HttpResponse *);

private:
	netcore::EventLoop *loop_;
	netcore::Server server_;
	HttpCallback httpCallback_;
};


#endif