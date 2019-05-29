#include "httpserver.h"
#include <functional>
#include "../../utils/timestamp.h"
#include "../../utils/logger.h"
#include "../../net/codec/codec.h"
#include "../../net/codec/http/httpcodec.h"
using namespace netcore;

HttpServer::HttpServer(EventLoop *loop, const NetAddr &addr, const std::string &name)
	:loop_(loop),
	server_(loop, addr, name)
{
	server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
	/*server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));*/
}

HttpServer::~HttpServer()
{
	stop();
}

void HttpServer::onConnection(const ConnectionPtr &conn)
{
	if (conn->isConnected())
	{
		HttpCodec *codec = new HttpCodec(HTTP_REQUEST);
		codec->setHttpCallback(std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		
		conn->setConnectionCodec(static_cast<Codec*>(codec));
		conn->setMessageCallback(std::bind(&HttpCodec::onMessage, codec, std::placeholders::_1, std::placeholders::_2));
	}
}

void HttpServer::onMessage(const netcore::ConnectionPtr &conn, const httpparser::HttpMessage *msg, httpparser::HttpResponse *rsp)
{
	LOG_INFO << "HttpCodec Send Message: " << msg->toStr();
	rsp->setStatus(HTTP_STATUS_OK);
	rsp->setKeepAlive(true, true);
}