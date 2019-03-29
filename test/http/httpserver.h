#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H
#include "httprequest.h"
#include "httpresponse.h"
#include "httpcontext.h"
#include "../../utils/noncopyable.h"
#include "../../net/eventloop.h"
#include "../../net/server.h"
#include "../../net/callback.h"
#include "../../net/buffer.h"
#include "../../net/connection.h"
using namespace netcore;

class HttpServer : NonCopyable
{
public:
	typedef std::function<void(const HttpRequest &, HttpResponse *)> HttpCallback;
	HttpServer(EventLoop *loop, const NetAddr &addr, std::string &name);
	~HttpServer();

	void start(int threadnum = 0) { server_.start(threadnum); }
	void stop() { server_.stop(); }

	void setHttpCallback(const HttpCallback &cb) { httpCallback_ = cb; }

private:
	void onRequest(const ConnectionPtr &conn, const HttpRequest &);
	void onMessage(const ConnectionPtr &conn, Buffer &buffer);
	void onConnection(const ConnectionPtr &conn);
	void defalutHttpCallback(const HttpRequest &, HttpResponse *);

private:
	EventLoop *loop_;
	Server server_;
	HttpCallback httpCallback_;
	std::map<int, HttpContext> ctx_;
};


#endif