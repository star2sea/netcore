#include <assert.h>
#include "server.h"

using namespace netcore;

Server::Server(EventLoop *loop, const NetAddr & addr, std::string &name)
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
			threads_.push_back(std::unique_ptr<EventLoopThread>(new EventLoopThread(addr_, connectionCallback_, messageCallback_)));
		}
	}
	else
	{
		acceptor_ = std::make_shared<Acceptor>(loop_, addr_);
		acceptor_->setMessageCallback(messageCallback_);
		acceptor_->setConnectionCallback(connectionCallback_);
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