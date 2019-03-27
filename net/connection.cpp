#include "connection.h"
#include "channel.h"
using namespace netcore;

Connection::Connection(EventLoop *loop, int connfd)
	:loop_(loop),
	sock_(connfd),
	connChannel_(new Channel(connfd, loop)),
	state_(Connecting)
{
	connChannel_->setReadableCallback(std::bind(&Connection::handleReadable, this));
	connChannel_->setWritableCallback(std::bind(&Connection::handleWritable, this));
}

Connection::~Connection()
{
	sock_.close();
    delete connChannel_;
}

void Connection::connectionEstablished()
{
	assert(state_ == Connecting);
	state_ = Connected;

	if (connectionCallback_)
		connectionCallback_(shared_from_this());

	connChannel_->enableReading();
	// todo enableWriting()
	
}

void Connection::connectionDestroyed()
{
    state_ = Disconnected;
    
    connChannel_->disableAll();
    
    if (connectionCallback_)
        connectionCallback_(shared_from_this());
}

void Connection::handleReadable()
{
    
}

void Connection::handleWritable()
{

}

void Connection::send()
{

}