#include "connector.h"

using namespace netcore;

Connector::Connector(EventLoop *loop)
	:loop_(loop),
	connectorChannel_(sock_.fd(), loop),
	state_(Disconnected)
{
	connectorChannel_.setWritableCallback(std::bind(&Connector::handleWritable, shared_from_this()));
}
Connector::~Connector()
{
	disconnect();
}

void Connector::connect(const NetAddr & serveraddr)
{
	assert(state_ == Disconnected);
	loop_->assertInOwnThread();

	sock_ = Socket::createNonBlockingSocket();

	int err = 0;
	int ret = -1;

	do {
		ret = sock_.connect(serveraddr);
		err = sock_.getSocketError();
	} while (ret < 0 && (err == EAGAIN || err == EADDRINUSE || err == ECONNREFUSED));

	if (ret < 0)
	{
		if (err == EINPROGRESS)
		{
			state_ = Connecting;
			connectorChannel_.enableWriting();
		}
		else
		{
			std::cout << "Connector::connect error" << std::endl;
			sock_.close();
			return;
		}
	}
	else
	{
		onConnected();
	}
}

void Connector::onConnected()
{
	state_ = Connected;
	connectorChannel_.disableWriting();

	NetAddr peeraddr(sock_.getPeerAddr());
	connection_ = std::make_shared<Connection>(loop_, sock_.fd(), peeraddr);
	connection_->setConnectionCallback(connectionCallback_);
	connection_->setMessageCallback(messageCallback_);
	connection_->setClosedCallback(std::bind(&Connector::handleClosed, shared_from_this()));
	connection_->connectionEstablished();
}

void Connector::handleWritable()
{
	if (state_ == Connecting)
	{
		int err = sock_.getSocketError();
		if (err)
		{
			std::cout << "Connector::handleWritable error" << std::endl;
			sock_.close();
			return;
		}
		else
		{
			onConnected();
		}
	}
	else
	{
		sock_.close();
		return;
	}
}

void Connector::handleClosed()
{
	assert(state_ == Connected);
	disconnect();
}

void Connector::disconnect()
{
	if (state_ == Disconnected)
		return;
	state_ = Disconnected;
	connection_->connectionDestroyed();
	sock_.close();
}