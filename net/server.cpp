#include "server.h"
#include <assert.h>
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
			EventLoop * loop = new EventLoop();
			Acceptor *acceptor = newAcceptor(loop);
			threads_.emplace_back(acceptor); // todo thread初始化结束后立刻调用acceptor->startAccept()
		}
	}
	else
	{
		Acceptor * acceptor = newAcceptor(loop_);
		acceptor_->startAccept();
	}
}

void Server::stop()
{
	if (!running_)
		return;
	running_ = false;
	
	if (acceptor_)
		delete acceptor_;

	for (auto & thread : threads_)
	{
		thread.join(); // todo
	}
}

Acceptor * Server::newAcceptor(EventLoop *loop)
{
	Acceptor * acceptor = new Acceptor(loop, addr_);
	acceptor->setMessageCallback(messageCallback_);
	acceptor->setConnectionCallback(connectionCallback_);
	return acceptor;
}
