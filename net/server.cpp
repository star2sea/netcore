#include <assert.h>
#include "server.h"

using namespace netcore;

Server::Server(EventLoop *loop, const NetAddr & addr, const std::string &name)
	:loop_(loop),
	addr_(addr),
	name_(name),
	acceptor_(NULL),
	running_(false)
{

}

Server::~Server()
{
	stop();
}

void Server::start(int threadnum)
{
	running_ = true;

	if (threadnum > 0)
	{
		for (int i = 0; i < threadnum; ++i)
		{
			threads_.push_back(std::unique_ptr<EventLoopThread>(new EventLoopThread(
				addr_, 
				std::bind(&Server::onConnectionWrapper, this, std::placeholders::_1),
				std::bind(&Server::onMessageWrapper, this, std::placeholders::_1, std::placeholders::_2)
			)));
		}
	}
	else
	{
		acceptor_ = std::make_shared<Acceptor>(loop_, addr_);
		acceptor_->setMessageCallback(std::bind(&Server::onMessageWrapper, this, std::placeholders::_1, std::placeholders::_2));
		acceptor_->setConnectionCallback(std::bind(&Server::onConnectionWrapper, this, std::placeholders::_1));
		acceptor_->startAccept();
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

void Server::onConnectionInLoop(const ConnectionPtr &conn)
{
	loop_->assertInOwnThread();
	//printf("new connection in loop\n");
	connectionCallback_(conn);
}
void Server::onMessageInLoop(const ConnectionPtr &conn, Buffer &buffer)
{
	loop_->assertInOwnThread();
	//printf("new message in loop\n");
	messageCallback_(conn, buffer);
}