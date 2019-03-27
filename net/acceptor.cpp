#include "acceptor.h"
#include "channel.h"
#include "connection.h"

using namespace netcore;

Acceptor::Acceptor(EventLoop * loop, const NetAddr &addr)
	:loop_(loop),
	addr_(addr),
	sock_(Socket::createNonBlockingSocket()),
	acceptChannel_(new Channel(sock_.fd(), loop_)),
	accepting_(false)
{
    sock_.setReuseAddr(true);
    sock_.setReusePort(true);
    sock_.setTcpNoDelay(true);
    
    acceptChannel_->setReadableCallback(std::bind(&Acceptor::handleReadable, this));
}

Acceptor::~Acceptor()
{
	stopAccept();
    sock_.close();
}

void Acceptor::startAccept()
{
	loop_->assertInOwnThread();
	sock_.bind(addr_);
	sock_.listen(1000);
	acceptChannel_->enableReading();
}

void Acceptor::stopAccept()
{
	loop_->assertInOwnThread();

	if (!accepting_)
		return;
	accepting_ = false;
	acceptChannel_->disableAll();
	delete acceptChannel_;  // channel update todo

	// close connections todo
}

void Acceptor::handleReadable()
{
	loop_->assertInOwnThread();
	int connfd = sock_.accept(); // todo
	if (connfd > 0)
	{
		onNewConnection(connfd);
	}
	else
	{
		// todo error
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

void Acceptor::onNewConnection(int connfd)
{
	loop_->assertInOwnThread();
	assert(connections_.find(connfd) == connections_.end());
	std::shared_ptr<Connection> conn = std::make_shared<Connection>(connfd);

	conn->setMessageCallback(messageCallback_);
	conn->setConnectionCallback(connectionCallback_);
	conn->setClosedCallback(std::bind(&Acceptor::handleClosed, this, std::placeholders::_1));

	connections_[connfd] = conn;

	conn->connectionEstablished();
}
