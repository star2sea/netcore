#include "acceptor.h"
#include "channel.h"
#include "connection.h"

using namespace netcore;

Acceptor::Acceptor(EventLoop * loop, const NetAddr &addr)
	:loop_(loop),
	addr_(addr),
	sock_(Socket::createNonBlockingSocket()),
	acceptChannel_(sock_.fd(), loop_),
	accepting_(false)
{
    sock_.setReuseAddr(true);
    sock_.setReusePort(true);
    sock_.setTcpNoDelay(true);
}

Acceptor::~Acceptor()
{
	sock_.close();
}

void Acceptor::startAccept()
{
	loop_->assertInOwnThread();
	std::cout << "server bind " << addr_.toIpPort() << std::endl;
	sock_.bind(addr_);
	sock_.listen(1000);
	acceptChannel_.setReadableCallback(std::bind(&Acceptor::handleReadable, shared_from_this()));
	acceptChannel_.enableReading();
}

void Acceptor::stopAccept()
{
	if (loop_->inOwnThread())
	{
		stopAcceptInLoop();
	}
	else
	{
		loop_->runInLoop(std::bind(&Acceptor::stopAcceptInLoop, shared_from_this()));
	}
}

void Acceptor::stopAcceptInLoop()
{
	loop_->assertInOwnThread();
	if (!accepting_)
		return;
	accepting_ = false;
	acceptChannel_.disableAll();
	acceptChannel_.remove();
	
	for (auto iter = connections_.begin(); iter != connections_.end(); ++iter)
	{
		iter->second->connectionDestroyed();
	}
	connections_.clear();
}

void Acceptor::handleReadable()
{
	loop_->assertInOwnThread();
	NetAddr peeraddr;
	int connfd = sock_.accept(&peeraddr);
	if (connfd > 0)
	{
		Socket::setNonBlocking(connfd);
		onNewConnection(connfd, peeraddr);
	}
	else
	{
		std::cout << "Acceptor::handleReadable error" << std::endl;
	}
}

void Acceptor::handleClosed(const ConnectionPtr & conn)
{
    loop_->assertInOwnThread();
    int fd = conn->fd();
    assert (connections_.find(fd) != connections_.end());
    connections_.erase(fd);
    conn->connectionDestroyed();
}

void Acceptor::onNewConnection(int connfd, NetAddr & peeraddr)
{
	loop_->assertInOwnThread();
	assert(connections_.find(connfd) == connections_.end());

	std::shared_ptr<Connection> conn = std::make_shared<Connection>(loop_, connfd, peeraddr);

	conn->setMessageCallback(messageCallback_);
	conn->setConnectionCallback(connectionCallback_);
	conn->setClosedCallback(std::bind(&Acceptor::handleClosed, shared_from_this(), std::placeholders::_1));

	connections_[connfd] = conn;

	conn->connectionEstablished();
}
