#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H
#include "http_parser/httpparser.h"
#include "../../utils/noncopyable.h"
#include "../../net/eventloop.h"
#include "../../net/server.h"
#include "../../net/callback.h"
#include "../../net/buffer.h"
#include "../../net/connection.h"

class HttpServer : netcore::NonCopyable
{
public:
	HttpServer(netcore::EventLoop *loop, const netcore::NetAddr &addr, const std::string &name);
	~HttpServer();

	void start(int threadnum = 0) { server_.start(threadnum); }
	void stop() { server_.stop(); }

private:
	void onMessage(const netcore::ConnectionPtr &conn, const httpparser::HttpMessage *, httpparser::HttpResponse *);
	void onConnection(const netcore::ConnectionPtr &conn);

private:
	netcore::EventLoop *loop_;
	netcore::Server server_;
};


#endif