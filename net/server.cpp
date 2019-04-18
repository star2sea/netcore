#include <assert.h>
#include "server.h"
#include "connection.h"
#include "../utils/logger.h"

using namespace netcore;

Server::Server(EventLoop *loop, const NetAddr & addr, const std::string &name)
	:loop_(loop),
	addr_(addr),
	name_(name),
	running_(false),
	connectionCallback_(std::bind(&Server::defaultConnectionCallback, this, std::placeholders::_1)),
	messageCallback_(std::bind(&Server::defaultMessageCallback, this, std::placeholders::_1, std::placeholders::_2))
{

}

Server::~Server()
{
	stop();
}

void Server::start(int threadnum)
{
	running_ = true;

	acceptor_ = std::unique_ptr<Acceptor>(new Acceptor(loop_, addr_));
	acceptor_->setMessageCallback(messageCallback_);
	acceptor_->setConnectionCallback(connectionCallback_);
	acceptor_->startAccept();

	if (threadnum > 0)
	{
		for (int i = 0; i < threadnum; ++i)
		{
			threads_.push_back(std::unique_ptr<EventLoopThread>(new EventLoopThread(
				addr_, 
				connectionCallback_,
				messageCallback_
			)));
		}
	}
}

void Server::stop()
{
	if (!running_)
		return;
	running_ = false;
	
	if (acceptor_)
		acceptor_->stopAccept();

	for (auto & loopthread : threads_)
	{
		loopthread->join();
	}
}

void Server::defaultConnectionCallback(const ConnectionPtr &conn)
{
	if (conn->isConnected())
	{
		LOG_TRACE << "new Connection";
	}
	else
	{
		LOG_TRACE << "Connection disconnected";
	}
}

void Server::defaultMessageCallback(const ConnectionPtr &conn, Buffer &buffer)
{
	buffer.consumeAll();
}